// slg2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "slg2.h"

#include "MainFrm.h"
#include "slg2Doc.h"
#include "slg2View.h"
#include "MainView.h"

#include "SlgNewAverager.h"
#include "SlgGroupNewAverager.h"

#include "Serial.h"
#include "AnalogueParamsConstList.h"
#include "DecCoeffCalcParams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//КОЛЬЦЕВОЙ БУФЕР БАЙТ ПРИХОДЯЩИХ В COM-ПОРТ
#define CYCLE_BUFFER_LEN 32768

char gl_bCircleBuffer[ CYCLE_BUFFER_LEN];
int gl_nCircleBufferGet;
int gl_nCircleBufferPut;
bool gl_PutByteInCircleBuffer( BYTE bt);

int gl_GetCircleBufferDistance( void)
{
  return( ( CYCLE_BUFFER_LEN + gl_nCircleBufferPut - gl_nCircleBufferGet) % CYCLE_BUFFER_LEN);
}

bool PutByteInCircleBuffer(BYTE bt)
{
  if( gl_nCircleBufferPut == gl_nCircleBufferGet - 1) return false;
  gl_bCircleBuffer[ gl_nCircleBufferPut] = bt;
  gl_nCircleBufferPut = ( ++gl_nCircleBufferPut) % CYCLE_BUFFER_LEN;
  return true;
}


//Время прошедешее с момента начала измерения
double gl_dGlobalTime;

//текущие (обновляемые ежетактно переменные)
double gl_dblW_tact;
double gl_dblTD1_tact, gl_dblTD2_tact, gl_dblTD3_tact;
double gl_dblI1_tact, gl_dblI2_tact;
double gl_dblVpc_tact;
double gl_dblAA_tact;

//Усреднители
CSlgGroupNewAverager gl_avgW;
CSlgGroupNewAverager gl_avgI1;
CSlgGroupNewAverager gl_avgI2;
CSlgGroupNewAverager gl_avgVpc;
CSlgGroupNewAverager gl_avgAmplAlt;
CSlgGroupNewAverager gl_avgAmplDus;
CSlgGroupNewAverager gl_avgAmplRULA;
CSlgGroupNewAverager gl_avgT1;
CSlgGroupNewAverager gl_avgT2;
CSlgGroupNewAverager gl_avgT3;
CSlgGroupNewAverager gl_avgTsa;

CDecCoeffCalcParams gl_pDecCoeffCalcParams;

/////////////////////////////////////////////////////////////////////////////
// ПОТОК РАСПРЕДЕЛЕНИЯ ДАННЫХ

BOOL m_bStopBigThreadFlag;

DWORD WINAPI BigThread(LPVOID lparam)
{
	bool bFirstPoint = true;
	double Kcoeff = 1.0;
	CString strError;
	
	BOOL bFirst100msecPointSkipped = false;

	m_bStopBigThreadFlag = false;

  BOOL bInveracityTact = FALSE;
  BOOL bInveracity100ms = FALSE;
  BOOL bInveracity1s = FALSE;
  BOOL bInveracity10s = FALSE;
  BOOL bInveracity100s = FALSE;

  int btPrevPackCounter = 500;

	while( !m_bStopBigThreadFlag) {
		int distance = ( CYCLE_BUFFER_LEN + gl_nCircleBufferPut - gl_nCircleBufferGet) % CYCLE_BUFFER_LEN;

		if( distance > 50) {			
			BYTE byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11, byte12;
      BYTE btCheckSumm = 0;
			short param, nCur1, nSaTime;
			double dAngle_inc, dCur1, dSaTime;
			
			int nMarkerCounter = 0;
      BOOL bMarkerFailOnce = true;
			do {

				//прокручиваем буфер выискивая последовательные 2 байта маркера (0x55, 0xAA)
				if( ( ( CYCLE_BUFFER_LEN + gl_nCircleBufferPut - gl_nCircleBufferGet) % CYCLE_BUFFER_LEN) > 20) {
					byte1 = gl_bCircleBuffer[ gl_nCircleBufferGet];
					gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;

          switch( nMarkerCounter) {
            case 0:
              if( byte1 == 0x55)
                nMarkerCounter = 1;
              else if( byte1 == 0x11)
                nMarkerCounter = 2;
              else {
                if( bMarkerFailOnce) {
                  theApp.m_nMarkerFails++;
                  bMarkerFailOnce = false;
                }
              }
            break;

            case 1:
              if( byte1 == 0xAA) {
                nMarkerCounter = 3;
              }
              else {
                nMarkerCounter = 0;
                if( bMarkerFailOnce) {
                  theApp.m_nMarkerFails++;
                  bMarkerFailOnce = false;
                }
              }
            break;

            case 2:
              if( byte1 == 0x88)
                nMarkerCounter = 3;
              else {
                nMarkerCounter = 0;
                if( bMarkerFailOnce) {
                  theApp.m_nMarkerFails++;
                  bMarkerFailOnce = false;
                }
              }
            break;
          }
				} 
			} while( nMarkerCounter != 3);

      
      //ПРИРАЩЕНИЕ УГЛА: 4 байта
			byte1 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			byte2 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			byte3 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			byte4 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;

			//НОМЕР ЧЕРЕДУЮЩЕГОСЯ (ТЕХНОЛОГИЧЕСКОГО, АНАЛОГОВОГО) ПАРАМЕТРА. 1 байт
			byte5 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			param = byte5;
	
			//ЗНАЧЕНИЕ ТЕХНОЛОГИЧЕСКОГО (АНАЛОГОВОГО) ПАРАМЕТРА. 2 Байта
			byte6 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			byte7 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			nCur1 = ( byte7 << 8) + byte6;
			dCur1 = ( double) (unsigned short) nCur1;
			
      /*
			if( ( byte5 & 0x7f) == 16) {
				( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer.Format(_T("v%d.%d.%d.0"), 
							(byte6 / 16),		//major
							byte6 % 16,			//middle
							byte7 / 16);		//minor

			}
      */

			//SA TIMING.
      //ИНТЕРВАЛ ВРЕМЕНИ МЕЖДУ ТЕКУЩИМ И ПРЕДЫДУЩИМ МОМЕНТАМИ ФИКСАЦИИ ПАРАМЕТРОВ. 2 БАЙТА
			byte8 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			byte9 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			nSaTime = ( byte9 << 8) + byte8;
			dSaTime = nSaTime;

      //ПОРЯДКОВЫЙ НОМЕР СООБЩЕНИЯ. 1 БАЙТ
      byte10 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;

			//EMERGENCY CODE
      //КОД ОШИБКИ. 1 БАЙТ
			byte11 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			
			
      //CHEKSUMM
      //КОНТРОЛЬНАЯ СУММА, CS. 1 байт
			byte12 = gl_bCircleBuffer[ gl_nCircleBufferGet];
			gl_nCircleBufferGet = ( gl_nCircleBufferGet + 1) % CYCLE_BUFFER_LEN;
			
      btCheckSumm = byte1;
      btCheckSumm += byte2;
      btCheckSumm += byte3;
      btCheckSumm += byte4;
      btCheckSumm += byte5;
      btCheckSumm += byte6;
      btCheckSumm += byte7;
      btCheckSumm += byte8;
      btCheckSumm += byte9;
      btCheckSumm += byte10;
      btCheckSumm += byte11;
      
      if( btCheckSumm != byte12) {
        //TODO несовпадение контрольной суммы

        theApp.m_nCheckSummFails++;

        /*
        CString strMsg;
        strMsg.Format( _T("\nBytes:\n%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\nCS(mc): %02x\nCS(pc): %02x"),
                byte1, byte2, byte3, byte4, byte5, byte6,
                byte7, byte8, byte9, byte10, byte11, byte12, btCheckSumm);
        AfxMessageBox( _T("Несовпадение контрольной суммы! FAIL!") + strMsg);
        */
        
        continue;

        //PostQuitMessage( 0);
      }

      //ОБРАБОТКА БАЙТА ОШИБКИ
      char bVeracity  = ( byte11 & 0x80) >> 7;
      char bLockBit   = ( byte11 & 0x40) >> 6;
      char bSyncAsync = ( byte11 & 0x20) >> 5;
      char bdWdNdU    = ( byte11 & 0x10) >> 4;
      char bErrorCode = byte11 & 0x0F;

      bInveracityTact   |= bVeracity;
      bInveracity100ms  |= bVeracity;
      bInveracity1s     |= bVeracity;
      bInveracity10s    |= bVeracity;
      bInveracity100s   |= bVeracity;

      //локбит
      if( theApp.m_bLockBit != bLockBit) {
        theApp.m_bLockBit = bLockBit;
      }

      //флаг синхр/асинхр режим работы прибора
      theApp.m_bSyncAsync = bSyncAsync;
      
      //флаг выдаваемого параметра
      theApp.m_bWdNdU = bdWdNdU;

      //пол-байта с кодом ошибки
      if( theApp.m_nEmergencyCode == 0)
				theApp.m_nEmergencyCode = bErrorCode;

      //проверка счётчика посылок
      if( btPrevPackCounter == 500) {
        btPrevPackCounter = byte10;
      }
      else {
        if( ( ( btPrevPackCounter + 1) % 256) != byte10) {
          theApp.m_nCounterFails++;
        }
        btPrevPackCounter = byte10;
      }

      //РАЗБОР ПАЧКИ
      //ПРИРАЩЕНИЕ УГЛА: 4 байта
      short dN, dU;
      if( bdWdNdU) {
        //режим рассчёта коэффициента вычета
        //От гироскопа нам передаются dN (short) и dU(short)
        
        
        //И нам нужен коэффициент вычета, а то без него мы не сможем вычислить Phi
        if( theApp.m_shFlashDecCoeff == 0) {
          if( byte5 == DECCOEFF)
            theApp.m_shFlashDecCoeff = nCur1;
          else
            continue;
        }

        //и нам нужен знаковый коэффициент
        if( theApp.m_shSignCoeff == 0) {
          if( byte5 == SIGNCOEFF)
            theApp.m_shSignCoeff = (( signed short) nCur1) - 1;
          else
            continue;
        }
        

        char *ptr;
        ptr = ( char *) &dN;
			  ptr[0] = byte1;
			  ptr[1] = byte2;

        ptr = ( char *) &dU;
			  ptr[0] = byte3;
			  ptr[1] = byte4;

        //CString strDbg;
        //strDbg.Format( "%d %d\n", dN, dU);
        //OutputDebugString( strDbg);

        if( gl_pDecCoeffCalcParams.m_bDecCoeffCalculation) {
          gl_pDecCoeffCalcParams.AddDnDuDtPoint( dN, dU, dSaTime);
        }

        dAngle_inc =  ( double) dN - ( ( double) dU * ( ( double) theApp.m_shFlashDecCoeff / 65535.)) * theApp.m_shSignCoeff;

        if( fabs( dAngle_inc) > 100.) {
          CString strTmp;
          strTmp.Format( _T("OOPS! dN:%d dU:%d\n"), dN, dU);
          OutputDebugString( strTmp);
        }
      }
      else {
        //рабочий режим

			  //float f_dN;
        int n_dN;
        char *ptr;      
        //ptr = ( char *) &f_dN;
        ptr = ( char *) &n_dN;
			  ptr[0] = byte1;
			  ptr[1] = byte2;
			  ptr[2] = byte3;
			  ptr[3] = byte4;

			  dAngle_inc = ( ( double) n_dN / 2147483647. * 99310.);
      }

      
      //осреднители
      gl_avgW.CommonAddPoint( dAngle_inc);

			/*
      if( param & 0x80)
				Beep( 1000, 10);
      */

      CString str;

			switch( byte5) {
				case UTD1:
          gl_avgT1.CommonAddPoint( dCur1);
          gl_dblTD1_tact = dCur1 / 65535. * 200. - 100.;
        break;
        case UTD2:
          gl_avgT2.CommonAddPoint( dCur1);
          gl_dblTD2_tact = dCur1 / 65535. * 200. - 100.;
        break;
        case UTD3:
          gl_avgT3.CommonAddPoint( dCur1);
          gl_dblTD3_tact = dCur1 / 65535. * 200. - 100.;
        break;


        case I1:
          gl_avgI1.CommonAddPoint( dCur1);
          gl_dblI1_tact = ( 2.5 - dCur1 / 4096. * 3.) / 2.5;
        break;
				case I2:
          gl_avgI2.CommonAddPoint( dCur1);
          gl_dblI2_tact = ( 2.5 - dCur1 / 4096. * 3.) / 2.5;
        break;


				case CNTRPC:
          gl_avgVpc.CommonAddPoint( dCur1);
          
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            gl_dblVpc_tact = ( ( dCur1 / 4096. * 3.) - 2.048) * 100.;

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            gl_dblVpc_tact = 101. * (( dCur1 / 4096. * 2.5) - 1.23);

          
        break;


				case AMPLANG_ALTERA:
          gl_avgAmplAlt.CommonAddPoint( dCur1);
          
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            gl_dblAA_tact = dCur1 * theApp.GetSettings()->GetScaleCoeff() / 4.;

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            gl_dblAA_tact = dCur1 * theApp.GetSettings()->GetScaleCoeff() / 2.;

        break;
        case AMPLANG_DUS:
          gl_avgAmplDus.CommonAddPoint( dCur1);
        break;
        case RULA:
          gl_avgAmplRULA.CommonAddPoint( dCur1);
        break;

				case AMPLITUDE: theApp.m_btParam1 = nCur1;        break;        //Амплитуда колебаний виброподвеса
				case TACT_CODE: theApp.m_btParam2 = nCur1;        break;        //Код такта подставки
				case M_COEFF:   theApp.m_btParam3 = nCur1;        break;        //коэффициент ошумления
				case STARTMODE: theApp.m_btParam4 = nCur1;        break;        //Начальная мода
        case DECCOEFF:  theApp.m_shFlashDecCoeff = nCur1; break;      //коэффициент вычета

        case CONTROL_I1:  theApp.m_shFlashI1min = nCur1;  break;        //Контрольный ток I1
			  case CONTROL_I2:  theApp.m_shFlashI2min = nCur1;  break;        //Контрольный ток I2
				case CONTROL_AA:  theApp.m_shFlashAmplAng1min = nCur1; break;   //Контрольный сигнал раскачки AmplAng1
					
        case HV_APPLY_COUNT_TR: theApp.m_nHvAppliesThisRun = nCur1; break;  //число 3kV-импульсов поджига

				case SIGNCOEFF:
					/*if( ( nCur1 != 0) || ( nCur1 != 2))
						( ( CSlg2App *) AfxGetApp())->m_shSignCoeff = 1;
					else
						( ( CSlg2App *) AfxGetApp())->m_shSignCoeff = nCur1-1;*/
					( ( CSlg2App *) AfxGetApp())->m_shSignCoeff = (( signed short) nCur1) - 1;

          /*
          str.Format( "$nCur1:%d $SignCoeff:%d", ( signed short) nCur1, ( ( CSlg2App *) AfxGetApp())->m_shSignCoeff);
          AfxMessageBox( str);
          */

				  /*Beep(200, 100);*/
				break; //flashParamSignCoeff

				case VERSION:
					//( ( CSlg2App *) AfxGetApp())->m_nSoftwareVer = nCur1;				//версия ПО
					/*Beep( 300, 100);*/
          ( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer.Format(_T("v%d.%d.%d.0"), 
				    	( byte6 / 16),		//major
							  byte6 % 16,			//middle
							  byte7 / 16);		//minor
				break; 

        case DEVNUM:
          theApp.m_bDeviceSerialNumber = true;
          theApp.m_nDeviceSerialNumber = nCur1;
        break;

				case CALIB_T1:
					theApp.m_shFlashT1 = nCur1;
					theApp.m_shFlashT1 -= THERMO_CALIB_PARAMS_BASE;
					/*Beep( 400, 100);*/
				break;		//калибровка термодатчиков: min T

				case T1_TD1: theApp.m_shFlashTD1_1 = nCur1; /*Beep( 500, 100);*/break; //калибровка термодатчиков: TD1_minT
				case T1_TD2: theApp.m_shFlashTD2_1 = nCur1; /*Beep( 600, 100);*/break; //калибровка термодатчиков: TD2_minT
        case T1_TD3: theApp.m_shFlashTD3_1 = nCur1; /*Beep( 600, 100);*/break; //калибровка термодатчиков: TD3_minT

				case CALIB_T2:
					theApp.m_shFlashT2 = nCur1;
					theApp.m_shFlashT2 -= THERMO_CALIB_PARAMS_BASE;
					/*Beep( 700, 100);*/
				break;		//калибровка термодатчиков: max T

				case T2_TD1: theApp.m_shFlashTD1_2 = nCur1; /*Beep( 800, 100);*/break; //калибровка термодатчиков: TD1_maxT
				case T2_TD2: theApp.m_shFlashTD2_2 = nCur1; /*Beep( 900, 100);*/ break; //калибровка термодатчиков: TD2_maxT
        case T2_TD3: theApp.m_shFlashTD3_2 = nCur1; /*Beep( 900, 100);*/ break; //калибровка термодатчиков: TD3_maxT
					
			}

			/*
			//проверка калибровки термодатчиков
			if( ( byte5 & 0x7f) == 21) {
				if( ( ( CSlg2App *) AfxGetApp())->m_shFlashT1 >= MIN_T_THERMO_CALIBRATION &&
						( ( CSlg2App *) AfxGetApp())->m_shFlashT1 <= MAX_T_THERMO_CALIBRATION) {
					if( ( ( CSlg2App *) AfxGetApp())->m_shFlashT2 >= MIN_T_THERMO_CALIBRATION &&
							( ( CSlg2App *) AfxGetApp())->m_shFlashT2 <= MAX_T_THERMO_CALIBRATION) {
						
							
						( ( CSlg2App *) AfxGetApp())->m_bThermoCalibrated = TRUE;
							
							
						double x1, y1, x2, y2;

						x1 = ( ( CSlg2App *) AfxGetApp())->m_shFlashT1;
						x2 = ( ( CSlg2App *) AfxGetApp())->m_shFlashT2;

							
						y1 = ( ( CSlg2App *) AfxGetApp())->m_shFlashTD1_1;
						y2 = ( ( CSlg2App *) AfxGetApp())->m_shFlashTD1_2;

						( ( CSlg2App *) AfxGetApp())->m_dThermoCalibB_1TD = ( x2 * y1 - x1 * y2) / ( x1 - x2);
						( ( CSlg2App *) AfxGetApp())->m_dThermoCalibK_1TD = ( y2 - y1) / ( x2 - x1);


						y1 = ( ( CSlg2App *) AfxGetApp())->m_shFlashTD2_1;
						y2 = ( ( CSlg2App *) AfxGetApp())->m_shFlashTD2_2;

            ( ( CSlg2App *) AfxGetApp())->m_dThermoCalibB_2TD = ( x2 * y1 - x1 * y2) / ( x1 - x2);
            ( ( CSlg2App *) AfxGetApp())->m_dThermoCalibK_2TD = ( y2 - y1) / ( x2 - x1);
          }
        }
      }*/

      theApp.GetLogger()->LogDebug ("BigThread::Tact: 1. before+ gl_dGlobalTime=%f tSaSumm=%f", gl_dGlobalTime, gl_avgTsa.Get_100ms()->GetSumm());
      gl_avgTsa.CommonAddPoint( dSaTime);
      theApp.GetLogger()->LogDebug ("BigThread::Tact: 2. after+  gl_dGlobalTime=%f tSaSumm=%f", gl_dGlobalTime, gl_avgTsa.Get_100ms()->GetSumm());


      //1275 (ФАПЧ) * 32768Hz (осцилятор) / 16 (делитель) = 2611200 Hz        
      //double dbl1secInTacts = 2611200;

      //1275 (ФАПЧ) * 32736Hz !!!(-16 - нестабильность)!!! (осцилятор) / 16 (делитель) = 2606100 Hz
      //double dbl1secInTacts = 2606100;

      //1273 (ФАПЧ - похоже всё таки ошибка тут) * 32768Hz / 16 (делитель) = 2607104 Hz
      double dbl1secInTacts = 2607104;

      //**************************************************
      // Запись полученных данных (фильтрованных по MF, CF)
      // **************************************************
      CSlg2App *app = ( CSlg2App *) AfxGetApp();
      if( (( CSlg2App *) AfxGetApp())->fhNew != NULL) {
        
        //маркер
        fputc( 0x55, app->fhNew);
        fputc( 0xAA, app->fhNew);

        //приращение угла
        fputc( byte1, app->fhNew);
        fputc( byte2, app->fhNew);
        fputc( byte3, app->fhNew);
        fputc( byte4, app->fhNew);

        //индикатор доп. параметра
        fputc(  byte5, app->fhNew);
        
        //доп. параметр
        fputc(  byte6, app->fhNew);
        fputc(  byte7, app->fhNew);
        
        //время такта
        fputc(  byte8, app->fhNew);
        fputc(  byte9, app->fhNew);

        //порядковый номер
        fputc( byte10, app->fhNew);

        //флаги, код ошибки
        fputc( byte11, app->fhNew);
        
        //чексумма
        fputc( byte12, app->fhNew);
      }
      else
        AfxMessageBox( _T("Попытка записи данных в bin-файл с закрытым дескриптором."));


      //расчёт моментальной (тактовой) угловой скорости
      gl_dblW_tact = dAngle_inc / ( dSaTime / dbl1secInTacts);

      //глобальное время от запуска
      gl_dGlobalTime += dSaTime / dbl1secInTacts;


      //////////////////////////////////////////////////////////////////////
      //распихивание точек по кольцевым буферам (тактовые значения)
      //////////////////////////////////////////////////////////////////////
      theApp.m_tpW->Get_Tacts()->AddPoint(        gl_dblW_tact,   gl_dGlobalTime, bInveracityTact);
      theApp.m_tpI1->Get_Tacts()->AddPoint(       gl_dblI1_tact,  gl_dGlobalTime, bInveracityTact);
      theApp.m_tpI2->Get_Tacts()->AddPoint(       gl_dblI2_tact,  gl_dGlobalTime, bInveracityTact);
      theApp.m_tpVpc->Get_Tacts()->AddPoint(      gl_dblVpc_tact, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpAmplAng->Get_Tacts()->AddPoint(  gl_dblAA_tact,  gl_dGlobalTime, bInveracityTact);
      theApp.m_tpT1->Get_Tacts()->AddPoint(       gl_dblTD1_tact, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpT2->Get_Tacts()->AddPoint(       gl_dblTD2_tact, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpT3->Get_Tacts()->AddPoint(       gl_dblTD3_tact, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpTsaMs->Get_Tacts()->AddPoint(    dSaTime / dbl1secInTacts * 1.e3, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpTsaMcs->Get_Tacts()->AddPoint(   dSaTime / dbl1secInTacts * 1.e6, gl_dGlobalTime, bInveracityTact);
      theApp.m_tpTsaHz->Get_Tacts()->AddPoint(    dbl1secInTacts / dSaTime       , gl_dGlobalTime, bInveracityTact);
      theApp.m_tpDecCoeff->Get_Tacts()->AddPoint( theApp.m_shFlashDecCoeff / 65535., gl_dGlobalTime, bInveracityTact);

      bInveracityTact = FALSE;


      // **************************************************
      // ОСРЕДНЕНИЕ 100 мсек
      // **************************************************

      if( gl_avgTsa.Get_100ms()->GetSumm() > dbl1secInTacts / 10.) {
        //накопили 100мсек
        if( bFirst100msecPointSkipped) {

          //количество импульсов (суммированное)
          double Nimp = gl_avgW.Get_100ms()->GetSumm();
          gl_avgW.Get_100ms()->Reset();

          //разрядный ток i1
          double dbl_pi1;
          if( gl_avgI1.Get_100ms()->GetCounter()) {
            double i1 = gl_avgI1.Get_100ms()->GetMean();
            dbl_pi1 = ( 2.5 - i1 / 4096. * 3.) / 2.5;               // mA
          }

          //разрядный ток i2
          double dbl_pi2;
          if( gl_avgI2.Get_100ms()->GetCounter()) {
            double i2 = gl_avgI2.Get_100ms()->GetMean();						
            dbl_pi2 = ( 2.5 - i2 / 4096. * 3.) / 2.5;;							// mA
            //gl_pi2 = i2;
          }

          //напряжение на пьезокорректорах
          double dbl_pVpc;
          if( gl_avgVpc.Get_100ms()->GetCounter()) {
            double Vpc = gl_avgVpc.Get_100ms()->GetMean();					  //напряжение на пьезокорректорах
            
            if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
              dbl_pVpc = ( ( Vpc / 4096. * 3.) - 2.048) * 100.;

            if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
              dbl_pVpc = 101. * (( Vpc / 4096. * 2.5) - 1.23);
          }

          //амплитуда от альтеры (в импульсах)
          double dbl_pAA;
          if( gl_avgAmplAlt.Get_100ms()->GetCounter()) {
            double AmplAng = gl_avgAmplAlt.Get_100ms()->GetMean();	            //amplang
            
            if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
              dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff() / 4.;	//''

            if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
              dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff();	    //''
          }

          //амплитуда от ДУСа
          double dbl_pAADus;
          if( gl_avgAmplDus.Get_100ms()->GetCounter()) {
            double AmplAngDus = gl_avgAmplDus.Get_100ms()->GetMean();         //amplangDus
            dbl_pAADus = AmplAngDus / 4096. * 3.;							                // V
          }

          //амплитуда RULA
          double dbl_pRULA;
          if( gl_avgAmplRULA.Get_100ms()->GetCounter()) {
            dbl_pRULA = gl_avgAmplRULA.Get_100ms()->GetMean();                //amplangRULA
          }

          //температура 1
          double dbl_pT1;
          if( gl_avgT1.Get_100ms()->GetCounter()) {
            double T1 = gl_avgT1.Get_100ms()->GetMean();                      //термодатчик 1
            dbl_pT1 = T1 / 65535. * 200. - 100.;				                      //V!
            //gl_pT1 = T1;
          }


          //температура 2
          double dbl_pT2;
          if( gl_avgT2.Get_100ms()->GetCounter()) {
            double T2 = gl_avgT2.Get_100ms()->GetMean();	            //термодатчик 2
            dbl_pT2 = T2 / 65535. * 200. - 100.;				            //V!
            //gl_pT2 = T2;
          }

          /*
          if( gl_avT2->GetCounter()) {
            //double T2 = gl_avT2->GetMean();						//получаемая средняя амплитуда с альтеры
            //gl_pT2 = T2 * ( ( CSlg2App *) AfxGetApp())->m_dKimpSec;

            double T2 = gl_avT2->GetMean();						  //RULA
            gl_pT2 = T2;
          }
          */

          
          //температура 3
          double dbl_pT3;
          if( gl_avgT3.Get_100ms()->GetCounter()) {
            double T3 = gl_avgT3.Get_100ms()->GetMean();             //термодатчик 3
            dbl_pT3 = T3 / 65535. * 200. - 100.;				            //V!
            //gl_pT3 = T3;
          }


          //время такта
          double Tsa = gl_avgTsa.Get_100ms()->GetSumm();            //сумма времен Tsa (должно близится к 100мсек)
          double TsaMean = gl_avgTsa.Get_100ms()->GetMean();        //среднее время Tsa за эти 100мсек
          
          //////////////////////////////////////////////////////////////////////
          //пересчет в физические величины
          //////////////////////////////////////////////////////////////////////

          //время (100мсек)
          double dbl_pTsa = Tsa / dbl1secInTacts;                   // sec

          //угловая скорость в 100мсек осреднении
          double dbl_pw100 = Nimp / dbl_pTsa * theApp.GetSettings()->GetScaleCoeff() / 4.;				// ''/sec

          double dbl_pTSamean = TsaMean / dbl1secInTacts;			      // sec


          //разделитель 100мсек точек в статистике
          /*
          if( (( CSlg2App *) AfxGetApp())->fh != NULL)
            fprintf( (( CSlg2App *) AfxGetApp())->fh, _T("\n"));
          else
            AfxMessageBox( _T("Попытка записи в файл разделителя с закрытым дескриптором!"));
          */

          //////////////////////////////////////////////////////////////////////
          //распихивание точек по кольцевым буферам
          //////////////////////////////////////////////////////////////////////
          theApp.GetLogger()->LogDebug ("BigThread::100ms: dbl_pw100=%f tmoment=%f tsasumm=%f", dbl_pw100, gl_dGlobalTime, gl_avgTsa.Get_100ms()->GetSumm());

          theApp.m_tpW->Get_100ms()->AddPoint(        dbl_pw100,  gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpI1->Get_100ms()->AddPoint(       dbl_pi1,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpI2->Get_100ms()->AddPoint(       dbl_pi2,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpVpc->Get_100ms()->AddPoint(      dbl_pVpc,   gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpAmplAng->Get_100ms()->AddPoint(  dbl_pAA,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpAmplAngDus->Get_100ms()->AddPoint( dbl_pAADus, gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpT1->Get_100ms()->AddPoint(       dbl_pT1,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpT2->Get_100ms()->AddPoint(       dbl_pT2,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpT3->Get_100ms()->AddPoint(       dbl_pT3,    gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpTsaMcs->Get_100ms()->AddPoint(   dbl_pTSamean * 1000000.,  gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpTsaMs->Get_100ms()->AddPoint(    dbl_pTSamean * 1000.,     gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpTsaHz->Get_100ms()->AddPoint(    1. / dbl_pTSamean,        gl_dGlobalTime, bInveracity100ms);
          theApp.m_tpDecCoeff->Get_100ms()->AddPoint( theApp.m_shFlashDecCoeff / 65535., gl_dGlobalTime, FALSE);

          bInveracity100ms = FALSE;

        }
        else {
          //ПРОПУСК ПЕРВЫХ 100 msec
          gl_avgW.CommonReset();
          gl_avgI1.CommonReset();
          gl_avgI2.CommonReset();
          gl_avgVpc.CommonReset();
          gl_avgAmplAlt.CommonReset();
          gl_avgAmplDus.CommonReset();
          gl_avgAmplRULA.CommonReset();
          gl_avgT1.CommonReset();
          gl_avgT2.CommonReset();
          gl_avgTsa.CommonReset();
          

          //разделитель 100мсек точек в статистике
          //fprintf( (( CSlg2App *) AfxGetApp())->fh, _T("\n"));
          bFirst100msecPointSkipped = true;
        }
      }

      // **************************************************
      // ОСРЕДНЕНИЕ 1 сек
      // **************************************************
      if( gl_avgTsa.Get_1s()->GetSumm() > dbl1secInTacts) {
        //накопили секундную точку

        //количество импульсов (суммированное)
        double Nimp = gl_avgW.Get_1s()->GetSumm();
        gl_avgW.Get_1s()->Reset();

        //разрядный ток i1
        double dbl_pi1;
        if( gl_avgI1.Get_1s()->GetCounter()) {
          double i1 = gl_avgI1.Get_1s()->GetMean();
          dbl_pi1 = ( 2.5 - i1 / 4096. * 3.) / 2.5;                 // mA
        }

        //разрядный ток i2
        double dbl_pi2;
        if( gl_avgI2.Get_1s()->GetCounter()) {
          double i2 = gl_avgI2.Get_1s()->GetMean();
          dbl_pi2 = ( 2.5 - i2 / 4096. * 3.) / 2.5;                 // mA
        }

        //напряжение на пьезокорректорах
        double dbl_pVpc;
				if( gl_avgVpc.Get_1s()->GetCounter()) {
					double Vpc = gl_avgVpc.Get_1s()->GetMean();					      //напряжение на пьезокорректорах
					
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pVpc = ( ( Vpc / 4096. * 3.) - 2.048) * 100.;

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pVpc = 101. * (( Vpc / 4096. * 2.5) - 1.23);
				}

        //амплитуда от альтеры (в импульсах)
        double dbl_pAA;
				if( gl_avgAmplAlt.Get_1s()->GetCounter()) {
					double AmplAng = gl_avgAmplAlt.Get_1s()->GetMean();	              //amplang
					
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff() / 4.;	//''

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff();	    //''
				}

        //амплитуда от ДУСа
        double dbl_pAADus;
        if( gl_avgAmplDus.Get_1s()->GetCounter()) {
					double AmplAngDus = gl_avgAmplDus.Get_1s()->GetMean();  //amplangDus
					dbl_pAADus = AmplAngDus / 4096. * 3.;							      // V
				}

        //амплитуда RULA
        double dbl_pAARULA;
        if( gl_avgAmplRULA.Get_1s()->GetCounter()) {
					dbl_pAARULA = gl_avgAmplRULA.Get_1s()->GetMean();       //amplangRULA
				}

        //температура 1
        double dbl_pT1;
        if( gl_avgT1.Get_1s()->GetCounter()) {
					double T1 = gl_avgT1.Get_1s()->GetMean();               //термодатчик 1
					dbl_pT1 = T1 / 65535. * 200. - 100.;				            //V!
          //gl_pT1 = T1;
				}


        //температура 2
        double dbl_pT2;
        if( gl_avgT2.Get_1s()->GetCounter()) {
					double T2 = gl_avgT2.Get_1s()->GetMean();	              //термодатчик 2
					dbl_pT2 = T2 / 65535. * 200. - 100.;				            //V!
          //gl_pT2 = T2;
				}

          
        //температура 3
        double dbl_pT3;
        if( gl_avgT3.Get_1s()->GetCounter()) {
          double T3 = gl_avgT3.Get_1s()->GetMean();                //термодатчик 3
          dbl_pT3 = T3 / 65535. * 200. - 100.;				            //V!
          //gl_pT3 = T3;
        }

        
        double Tsa = gl_avgTsa.Get_1s()->GetSumm();               //сумма времен Tsa (должно близится к 1 сек)
        double dbl_pTSamean = gl_avgTsa.Get_1s()->GetMean();       //среднее время Tsa за эти 1 сек

        //пересчет в физические величины
        double tsa_1s = Tsa / dbl1secInTacts;
        double w_1s = Nimp / tsa_1s * theApp.GetSettings()->GetScaleCoeff() / 4.;

        //перекладывание в кольцевой буфер
        theApp.m_tpW->          Get_1s()->AddPoint( w_1s,       gl_dGlobalTime, bInveracity1s);
        theApp.m_tpI1->         Get_1s()->AddPoint( dbl_pi1,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpI2->         Get_1s()->AddPoint( dbl_pi2,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpVpc->        Get_1s()->AddPoint( dbl_pVpc,   gl_dGlobalTime, bInveracity1s);
        theApp.m_tpAmplAng->    Get_1s()->AddPoint( dbl_pAA,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpAmplAngDus-> Get_1s()->AddPoint( dbl_pAADus, gl_dGlobalTime, bInveracity1s);
        theApp.m_tpT1->         Get_1s()->AddPoint( dbl_pT1,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpT2->         Get_1s()->AddPoint( dbl_pT2,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpT3->         Get_1s()->AddPoint( dbl_pT3,    gl_dGlobalTime, bInveracity1s);
        theApp.m_tpTsaMcs->     Get_1s()->AddPoint( dbl_pTSamean / dbl1secInTacts * 1000000.,  gl_dGlobalTime, bInveracity1s);
        theApp.m_tpTsaMs->      Get_1s()->AddPoint( dbl_pTSamean / dbl1secInTacts* 1000.,     gl_dGlobalTime, bInveracity1s);
        theApp.m_tpTsaHz->      Get_1s()->AddPoint( dbl1secInTacts / dbl_pTSamean,        gl_dGlobalTime, bInveracity1s);
        theApp.m_tpDecCoeff->   Get_1s()->AddPoint( theApp.m_shFlashDecCoeff / 65535., gl_dGlobalTime, FALSE);

        bInveracity1s = FALSE;
      }

      // **************************************************
      // ОСРЕДНЕНИЕ 10 сек
      // **************************************************
      if( gl_avgTsa.Get_10s()->GetSumm() > dbl1secInTacts *10.) {
        //накопили 10секундную точку

        //количество импульсов (суммированное)
        double Nimp = gl_avgW.Get_10s()->GetSumm();
        gl_avgW.Get_10s()->Reset();

        //разрядный ток i1
        double dbl_pi1;
        if( gl_avgI1.Get_10s()->GetCounter()) {
          double i1 = gl_avgI1.Get_10s()->GetMean();
          dbl_pi1 = ( 2.5 - i1 / 4096. * 3.) / 2.5;                 // mA
        }

        //разрядный ток i2
        double dbl_pi2;
        if( gl_avgI2.Get_10s()->GetCounter()) {
          double i2 = gl_avgI2.Get_10s()->GetMean();
          dbl_pi2 = ( 2.5 - i2 / 4096. * 3.) / 2.5;                 // mA
        }

        //напряжение на пьезокорректорах
        double dbl_pVpc;
				if( gl_avgVpc.Get_10s()->GetCounter()) {
					double Vpc = gl_avgVpc.Get_10s()->GetMean();					            //напряжение на пьезокорректорах

					if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pVpc = ( ( Vpc / 4096. * 3.) - 2.048) * 100.;

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pVpc = 101. * (( Vpc / 4096. * 2.5) - 1.23);
				}

        //амплитуда от альтеры (в импульсах)
        double dbl_pAA;
				if( gl_avgAmplAlt.Get_10s()->GetCounter()) {
					double AmplAng = gl_avgAmplAlt.Get_10s()->GetMean();	            //amplang
					
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff() / 4.;	//''

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff();	    //''
				}

        //амплитуда от ДУСа
        double dbl_pAADus;
        if( gl_avgAmplDus.Get_10s()->GetCounter()) {
					double AmplAngDus = gl_avgAmplDus.Get_10s()->GetMean();           //amplangDus
					dbl_pAADus = AmplAngDus / 4096. * 3.;							                // V
				}

        //амплитуда RULA
        double dbl_pAARULA;
        if( gl_avgAmplRULA.Get_10s()->GetCounter()) {
					dbl_pAARULA = gl_avgAmplRULA.Get_10s()->GetMean();                //amplang RULA
				}

        //температура 1
        double dbl_pT1;
        if( gl_avgT1.Get_10s()->GetCounter()) {
					double T1 = gl_avgT1.Get_10s()->GetMean();                        //термодатчик 1
					dbl_pT1 = T1 / 65535. * 200. - 100.;				                      //V!
          //gl_pT1 = T1;
				}


        //температура 2
        double dbl_pT2;
        if( gl_avgT2.Get_10s()->GetCounter()) {
					double T2 = gl_avgT2.Get_10s()->GetMean();	                      //термодатчик 2
					dbl_pT2 = T2 / 65535. * 200. - 100.;				                      //V!
          //gl_pT2 = T2;
				}

          
        //температура 3
        double dbl_pT3;
        if( gl_avgT3.Get_10s()->GetCounter()) {
          double T3 = gl_avgT3.Get_10s()->GetMean();                //термодатчик 3
          dbl_pT3 = T3 / 65535. * 200. - 100.;				            //V!
          //gl_pT3 = T3;
        }

        //сумма времен Tsa (должно близится к 1 сек)
        double Tsa = gl_avgTsa.Get_10s()->GetSumm();
        double dbl_pTSamean = gl_avgTsa.Get_10s()->GetMean();       //среднее время Tsa за эти 10 сек

        //пересчет в физические величины
        double tsa_10s = Tsa / dbl1secInTacts;
        double w_10s = Nimp / tsa_10s * theApp.GetSettings()->GetScaleCoeff() / 4.;


        //перекладывание в кольцевой буфер
        theApp.m_tpW->          Get_10s()->AddPoint( w_10s,                   gl_dGlobalTime, bInveracity10s);
        theApp.m_tpI1->         Get_10s()->AddPoint( dbl_pi1,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpI2->         Get_10s()->AddPoint( dbl_pi2,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpVpc->        Get_10s()->AddPoint( dbl_pVpc,                gl_dGlobalTime, bInveracity10s);
        theApp.m_tpAmplAng->    Get_10s()->AddPoint( dbl_pAA,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpAmplAngDus-> Get_10s()->AddPoint( dbl_pAADus,              gl_dGlobalTime, bInveracity10s);
        //theApp.m_tpAmplAngRULA->Get_10s()->AddPoint( dbl_pAARULA,             gl_dGlobalTime, bInveracity10s);
        theApp.m_tpT1->         Get_10s()->AddPoint( dbl_pT1,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpT2->         Get_10s()->AddPoint( dbl_pT2,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpT3->         Get_10s()->AddPoint( dbl_pT3,                 gl_dGlobalTime, bInveracity10s);
        theApp.m_tpTsaMcs->     Get_10s()->AddPoint( dbl_pTSamean / dbl1secInTacts * 1000000., gl_dGlobalTime, bInveracity10s);
        theApp.m_tpTsaMs->      Get_10s()->AddPoint( dbl_pTSamean / dbl1secInTacts * 1000.,    gl_dGlobalTime, bInveracity10s);
        theApp.m_tpTsaHz->      Get_10s()->AddPoint( dbl1secInTacts / dbl_pTSamean,       gl_dGlobalTime, bInveracity10s);
        theApp.m_tpDecCoeff->   Get_10s()->AddPoint( theApp.m_shFlashDecCoeff / 65535., gl_dGlobalTime, FALSE);

        bInveracity10s = FALSE;
      }

      // **************************************************
      // ОСРЕДНЕНИЕ 100 сек
      // **************************************************
      if( gl_avgTsa.Get_100s()->GetSumm() > dbl1secInTacts * 100.) {
        //накопили 100секундную точку

        //количество импульсов (суммированное)
        double Nimp = gl_avgW.Get_100s()->GetSumm();
        gl_avgW.Get_100s()->Reset();

        //разрядный ток i1
        double dbl_pi1;
        if( gl_avgI1.Get_100s()->GetCounter()) {
          double i1 = gl_avgI1.Get_100s()->GetMean();
          dbl_pi1 = ( 2.5 - i1 / 4096. * 3.) / 2.5;                // mA
        }

        //разрядный ток i2
        double dbl_pi2;
        if( gl_avgI2.Get_100s()->GetCounter()) {
          double i2 = gl_avgI2.Get_100s()->GetMean();
          dbl_pi2 = ( 2.5 - i2 / 4096. * 3.) / 2.5;                 // mA
        }

        //напряжение на пьезокорректорах
        double dbl_pVpc;
				if( gl_avgVpc.Get_100s()->GetCounter()) {
					double Vpc = gl_avgVpc.Get_100s()->GetMean();					      //напряжение на пьезокорректорах

					if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pVpc = ( ( Vpc / 4096. * 3.) - 2.048) * 100.;

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pVpc = 101. * (( Vpc / 4096. * 2.5) - 1.23);
				}

        //амплитуда от альтеры (в импульсах)
        double dbl_pAA;
				if( gl_avgAmplAlt.Get_100s()->GetCounter()) {
					double AmplAng = gl_avgAmplAlt.Get_100s()->GetMean();	            //amplang
					
          if( theApp.m_strSoftwareVer.Left( 6) == _T("v3.2.5"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff() / 4.;	//''

          if( theApp.m_strSoftwareVer.Left( 6) == _T("v4.2.0"))
            dbl_pAA = AmplAng * theApp.GetSettings()->GetScaleCoeff();	    //''
				}

        //амплитуда от ДУСа
        double dbl_pAADus;
        if( gl_avgAmplDus.Get_100s()->GetCounter()) {
					double AmplAngDus = gl_avgAmplDus.Get_100s()->GetMean();          //amplangDus
					dbl_pAADus = AmplAngDus / 4096. * 3.;							                // V
				}

        //амплитуда RULA
        double dbl_pAARULA;
        if( gl_avgAmplRULA.Get_100s()->GetCounter()) {
					dbl_pAARULA = gl_avgAmplRULA.Get_100s()->GetMean();               //amplangRULA
				}

        //температура 1
        double dbl_pT1;
        if( gl_avgT1.Get_100s()->GetCounter()) {
					double T1 = gl_avgT1.Get_100s()->GetMean();                  //термодатчик 1
					dbl_pT1 = T1 / 65535. * 200. - 100.;				              //V!
          //gl_pT1 = T1;
				}


        //температура 2
        double dbl_pT2;
        if( gl_avgT2.Get_100s()->GetCounter()) {
					double T2 = gl_avgT2.Get_100s()->GetMean();	              //термодатчик 2
					dbl_pT2 = T2 / 65535. * 200. - 100.;				            //V!
          //gl_pT2 = T2;
				}

          
        //температура 3
        double dbl_pT3;
        if( gl_avgT3.Get_100s()->GetCounter()) {
          double T3 = gl_avgT3.Get_100s()->GetMean();                //термодатчик 3
          dbl_pT3 = T3 / 65535. * 200. - 100.;				            //V!
          //gl_pT3 = T3;
        }

        //сумма времен Tsa (должно близится к 1 сек)
        double Tsa = gl_avgTsa.Get_100s()->GetSumm();
        double dbl_pTSamean = gl_avgTsa.Get_100s()->GetMean();       //среднее время Tsa за эти 10 сек

        //пересчет в физические величины
        double tsa_100s = Tsa / dbl1secInTacts;
        double w_100s = Nimp / tsa_100s * theApp.GetSettings()->GetScaleCoeff() / 4.;


        //перекладывание в кольцевой буфер
        theApp.m_tpW->          Get_100s()->AddPoint( w_100s,                   gl_dGlobalTime, bInveracity100s);
        theApp.m_tpI1->         Get_100s()->AddPoint( dbl_pi1,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpI2->         Get_100s()->AddPoint( dbl_pi2,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpVpc->        Get_100s()->AddPoint( dbl_pVpc,                 gl_dGlobalTime, bInveracity100s);
        theApp.m_tpAmplAng->    Get_100s()->AddPoint( dbl_pAA,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpAmplAngDus-> Get_100s()->AddPoint( dbl_pAADus,               gl_dGlobalTime, bInveracity100s);
        //theApp.m_tpAmplAngRULA->Get_100s()->AddPoint( dbl_pAARULA,              gl_dGlobalTime, bInveracity100s);
        theApp.m_tpT1->         Get_100s()->AddPoint( dbl_pT1,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpT2->         Get_100s()->AddPoint( dbl_pT2,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpT3->         Get_100s()->AddPoint( dbl_pT3,                  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpTsaMcs->     Get_100s()->AddPoint( dbl_pTSamean / dbl1secInTacts * 1000000.,  gl_dGlobalTime, bInveracity100s);
        theApp.m_tpTsaMs->      Get_100s()->AddPoint( dbl_pTSamean / dbl1secInTacts * 1000.,     gl_dGlobalTime, bInveracity100s);
        theApp.m_tpTsaHz->      Get_100s()->AddPoint( dbl1secInTacts / dbl_pTSamean,        gl_dGlobalTime, bInveracity100s);
        theApp.m_tpDecCoeff->   Get_100s()->AddPoint( theApp.m_shFlashDecCoeff / 65535., gl_dGlobalTime, FALSE);

        bInveracity100s = FALSE;

      } //обработка 100сек точек

    } //если растояние кольцевого буфера > 50		
    else
      Sleep( 1);

  } //while жизни потока

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CSlg2App

BEGIN_MESSAGE_MAP(CSlg2App, CWinApp)
	//{{AFX_MSG_MAP(CSlg2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlg2App construction

CSlg2App::CSlg2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nEmergencyCode = 0;
	fh = NULL;
	fhNew = NULL;
	m_shSignCoeff = 0;
	//m_bThermoCalibrated = FALSE;
  m_bDeviceSerialNumber = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSlg2App object

CSlg2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CSlg2App initialization

BOOL CSlg2App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Alcor Laboratories"));
  LoadStdProfileSettings();	
  m_pSettings.LoadSettings();

  /*
	int kimpsec = GetProfileInt( _T("SETTINGS"), _T("K_IMP_SEC"), 1000);
	m_dKimpSec = kimpsec / 1000.;

  m_nComPort = GetProfileInt( _T("SETTINGS"), _T("COM_PORT"), 0);
	m_nComBaudrate = GetProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), 0);
	m_nControlButtons = GetProfileInt( _T("SETTINGS"), _T("CONTROL_BUTTONS"), 0);
  m_nSaveGraph1 = GetProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), 0);
  m_nSaveGraph2 = GetProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), 0);
  m_nSaveGraph3 = GetProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), 0);
  m_nSaveGraph4 = GetProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), 0);
  m_nSaveGraph5 = GetProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), 0);
  m_nSaveGraph6 = GetProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), 0);
  m_nSaveGraph7 = GetProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), 0);
  m_nSaveGraph8 = GetProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), 0);

  */

  m_pLogger.Init();
  GetLogger()->LogInfo( "CSlg2App::InitInstance(): in");

	SYSTEMTIME sysTime;
	GetSystemTime( &sysTime);
	COleDateTime time5(sysTime);
	strDirName = time5.Format( _T("%Y-%m-%d"));
	if( !::CreateDirectory( strDirName, NULL)) {
		long lErrorCode = GetLastError();
		if( lErrorCode != 183)
			strDirName = _T( ".\\");
		else
			strDirName += _T("\\");
	}
	else
		strDirName += _T("\\");

	//ОЧИСТКА КОЛЬЦЕВОГО БУФФЕРА ПРИЕМА ДАННЫХ
	for( int i=0; i<CYCLE_BUFFER_LEN; i++) {
		gl_bCircleBuffer[ i]= 0;
	}

  //кольцевые буфера
  int n = 1024;
  m_tpW  =        new CTrackedParam( n);
  m_tpI1 =        new CTrackedParam( n);
  m_tpI2  =       new CTrackedParam( n);
  m_tpVpc =       new CTrackedParam( n);
	m_tpAmplAng =   new CTrackedParam( n);
  m_tpAmplAngDus = new CTrackedParam( n);
	m_tpT1 =        new CTrackedParam( n);
	m_tpT2 =        new CTrackedParam( n);
  m_tpT3 =        new CTrackedParam( n);
	m_tpTsaMcs =    new CTrackedParam( n);
  m_tpTsaMs =     new CTrackedParam( n);
  m_tpTsaHz =     new CTrackedParam( n);
  m_tpDecCoeff =  new CTrackedParam( n);

	gl_dGlobalTime = 0.;

	/*
	DWORD id1;
	HANDLE hthread1 = ::CreateThread(0, 0, &BeeperThread, 0, 0, &id1);

	//ЗАПУСК ПОТОКА ДЛЯ ОБРАБОТКИ ДАННЫХ
	DWORD id2;
	HANDLE hthread2 = ::CreateThread(0, 0, &BigThread, 0, 0, &id2);
	*/

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	/*fh = NULL;
	CPropertySheet dlgWizard( _T("Новый запуск"), NULL, 0);
	
	CWizardSelComPortStep dlgPage1;
	CWizardSelRunTypeStep dlgPage2;
	
	dlgWizard.AddPage( &dlgPage1);
	dlgWizard.AddPage( &dlgPage2);

	dlgWizard.SetWizardMode();
	
	
	if( dlgWizard.DoModal() == ID_WIZFINISH) {

	}
	else
		return FALSE;*/

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSlg2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		//RUNTIME_CLASS(CSlg2View));
		RUNTIME_CLASS(CMainView));
	
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	/*SYSTEMTIME sysTime;  // Win32 time information
	GetLocalTime(&sysTime);
	CString strStatFileName;
	strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d.txt"),
													sysTime.wYear, sysTime.wMonth, sysTime.wDay,
													sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	fh = fopen( strStatFileName, "w");*/
	fh = NULL;
	fhNew = NULL;
	fhb = NULL;
	
  m_bSyncAsync = 2;
  m_bWdNdU = 2;

	//m_strSoftwareVer = _T("v0.0.0.0");
	m_strSoftwareVer = _T("unknown");

  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSlg2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSlg2App message handlers




int CSlg2App::ExitInstance() 
{
  m_bStopBigThreadFlag = true;

  m_pSettings.SaveSettings();
  /*
  int kimpsec = m_dKimpSec * 1000;
  WriteProfileInt( _T("SETTINGS"), _T("K_IMP_SEC"), kimpsec);
  WriteProfileInt( _T("SETTINGS"), _T("COM_PORT"), m_nComPort);
  WriteProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), m_nComBaudrate);

  WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), m_nSaveGraph1);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), m_nSaveGraph2);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), m_nSaveGraph3);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), m_nSaveGraph4);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), m_nSaveGraph5);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), m_nSaveGraph6);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), m_nSaveGraph7);
  WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), m_nSaveGraph8);
  */

  if( fh != NULL)
    fclose( fh);

  if( fhb != NULL)
    fclose( fhb);

  if( fhNew != NULL)
    fclose( fhNew);

  //кольцевые буфера
  m_tpW->FreeUnder();
  m_tpI1->FreeUnder();
  m_tpI2->FreeUnder();
  m_tpVpc->FreeUnder();
  m_tpAmplAng->FreeUnder();
  m_tpAmplAngDus->FreeUnder();
  m_tpT1->FreeUnder();
  m_tpT2->FreeUnder();
  m_tpT3->FreeUnder();
  m_tpTsaMcs->FreeUnder();
  m_tpTsaMs->FreeUnder();
  m_tpTsaHz->FreeUnder();
  m_tpDecCoeff->FreeUnder();

  delete m_tpW;   m_tpW   = NULL;
  delete m_tpI1;  m_tpI1  = NULL;
  delete m_tpI2;  m_tpI2  = NULL;
  delete m_tpVpc; m_tpVpc = NULL;
  delete m_tpAmplAng;     m_tpAmplAng = NULL;
  delete m_tpAmplAngDus;  m_tpAmplAngDus = NULL;
  delete m_tpT1;          m_tpT1 = NULL;
  delete m_tpT2;          m_tpT2 = NULL;
  delete m_tpT3;          m_tpT3 = NULL;
  delete m_tpTsaMcs;      m_tpTsaMcs = NULL;
  delete m_tpTsaMs;       m_tpTsaMs = NULL;
  delete m_tpTsaHz;       m_tpTsaHz = NULL;
  delete m_tpDecCoeff;    m_tpDecCoeff = NULL;

  return CWinApp::ExitInstance();
}

void CSlg2App::StartThreads()
{
	gl_nCircleBufferGet = 0;
	gl_nCircleBufferPut = 0;

	m_bStopBigThreadFlag = false;

	//ЗАПУСК ПОТОКА ДЛЯ ОБРАБОТКИ ДАННЫХ
	DWORD id2;
	HANDLE hthread2 = ::CreateThread( 0, 0, &BigThread, 0, 0, &id2);
}
