#ifndef ANALOGUE_PARAMETERS_LIST
#define ANALOGUE_PARAMETERS_LIST

#define SENDING_UTD1              0
#define SENDING_UTD2              1
#define SENDING_UTD3              2
#define SENDING_I1                3
#define SENDING_I2                4
#define SENDING_CNTRPC            5
#define SENDING_AMPLANG_ALTERA    6
#define SENDING_AMPLANG_DUS       7

#define SENDING_AMPLITUDE         8 //код амплитуды
#define SENDING_TACT_CODE         9 //код такта подставки
#define SENDING_M_COEFF          10 //Коэффициент M
#define SENDING_STARTMODE        11 //Начальная мода
#define SENDING_DECCOEFF         12 //Коэффициент вычета
#define SENDING_SIGNCOEFF        13 //Знаковый коэффициент

#define SENDING_VERSION          14 //Версия

#define SENDING_CONTROL_I1       15 //Контрольный ток поджига I1
#define SENDING_CONTROL_I2       16 //Контрольный ток поджига I2
#define SENDING_CONTROL_AA       17 //Контрольный сигнал раскачки AmplAng


#define SENDING_CALIB_T1         18 //Температура нижней температурной точки калибровки
#define SENDING_T1_TD1           19 //показания датчика TD1 на нижней температурной точке
#define SENDING_T1_TD2           20 //показания датчика TD2 на нижней температурной точке
#define SENDING_T1_TD3           21 //показания датчика TD3 на нижней температурной точке

#define SENDING_CALIB_T2         22 //Температура верхней температурной точки калибровки
#define SENDING_T2_TD1           23 //показания датчика TD1 на верхней температурной точке
#define SENDING_T2_TD2           24 //показания датчика TD2 на верхней температурной точке
#define SENDING_T2_TD3           25 //показания датчика TD3 на верхней температурной точке

#define SENDING_DEVNUM_BL        26 //Номер прибора. Младший байт
#define SENDING_DEVNUM_BH        27 //Номер прибора. Старший байт

#define SENDING_ORG_B1           28 //Организация.Байт1
#define SENDING_ORG_B2           29 //Организация.Байт2
#define SENDING_ORG_B3           30 //Организация.Байт3
#define SENDING_ORG_B4           31 //Организация.Байт4
#define SENDING_ORG_B5           32 //Организация.Байт5
#define SENDING_ORG_B6           33 //Организация.Байт6
#define SENDING_ORG_B7           34 //Организация.Байт7
#define SENDING_ORG_B8           35 //Организация.Байт8
#define SENDING_ORG_B9           36 //Организация.Байт9
#define SENDING_ORG_B10          37 //Организация.Байт10
#define SENDING_ORG_B11          38 //Организация.Байт11
#define SENDING_ORG_B12          39 //Организация.Байт12
#define SENDING_ORG_B13          40 //Организация.Байт13
#define SENDING_ORG_B14          41 //Организация.Байт14
#define SENDING_ORG_B15          42 //Организация.Байт15
#define SENDING_ORG_B16          43 //Организация.Байт16        БЕЗ завершающего 0 на конце!!!!!

#define SENDING_DATE_Y           44 //Дата.год
#define SENDING_DATE_M           45 //Дата.месяц
#define SENDING_DATE_D           46 //Дата.день

#define SENDING_HV_APPLY_THIS_RUN 47 //Длительность применения 3kV при включении
#define SENDING_HV_APPLY_SET     48 //Уставка длительности применения 3kV при включении
#endif