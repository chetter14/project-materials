<PRADIS 2023.2>
<Properties>
  <View=0,24,1288,900,1,0,0>
  <Grid=10,10,1>
  <DataSet=e154.dat>
  <DataDisplay=e154.dpl>
  <OpenDisplay=1>
  <Script=e154.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Название>
  <FrameText1=Чертил:>
  <FrameText2=Дата:>
  <FrameText3=Версия:>
  <BinData=ECemow7uAnjN9o4hi0DEja5pIzLYTZbZMvSa/lmA0ZMIUlZfTvTm09hlj0nzBl1PAUgKMf4cZlckBgmw/JPyMFeLNeBxqPY34TLM+BR0XE2/s0/TgmAxPvhMMCqcgPviZJlp7XXazIhYYXQu2+CxHZyfBcJnUl0Uoh1y5aiHp15Kzi3qfIGIC8ZU/wwWNeeqI8e+DGdoMj7Jf85L7yMq1Gq5+v5tblngX+CmTeQoCyb1qe+o9m2w2kdHnT7ddqhZaiKNMRPqtdUd+WJJsvkZTWE3hxRw2fbOLgmIK3rVVp/OIj1W8axfO3gpGbCyY1JwhajVVFT5iEqxBBb0eHccmLxWkaBT6OzywELO2lmltDRhKwlvU1j3m324tv/Vz4VX8J6HIfDWwNHEgPmVdw9c06CskuUQtR/uI1Hdiv76ke9ifFj85Rud6vfKSmyvp2h+WKyaVt0F0z2djvDM20PN13o8go3z8Ww1nS1gluhWDl3yL1IbRKY2AlfjTy5nrUrows7Ai/rLdGyNP43Pkv13ky+MWobb+QFUTEIF3nscgvd+PRfqmfcsckBQpgsmKgnUBVJtDQAh/XSlmaj/2Us+EpWrKl5roGe8piuJ5GX+MmpDhcG3wll7MAXeZ7207qOecTTGTCZHLeX4RufhW8GpWk+kmKGIoUsb7XzJzpGnbro3Ri38juI54KpG8sVhLyxvYulczUPfuNoCQRkoyO7DfL0ExzAd2EHEFLbQqkI9XAoZifAvYqrgf6HRy1mLeo6K/+zImhz2fmyupU7f6qFYek84mwuzhnEkAQHjZM2KNN/IymSLiybjoJ9249mOmXB1obL2A/ADG8M+t5t+ZwJ7sGT3P4BfNnYGwM96KIXCtmU4WoBaSpqnpCgo//nbLQSn83/xtn3FrvZLYtWg8Tr9+gSZ8zNY/EKI0NPRoAmJkcLaCWffol+4t4y+PrwdOgUfXd32LNKbnUMoNmmvyZVii0yeV2IVby1Jh94Egjc0fAv0cKQpXljiYfeVsG4hpNGzziEhto3QXtLTXKF4/+SsNWV81Cv8u3LXfh77Tj0oDCJ5cRB6IBd4XbtcSCmB+Mr80itrLQI1Ok3ePultbQoyHuZi9l7SxkRK5NCqmBaj8tSpZ2M6iR0b2BeyV2AibIN8MQNgurGgUgBKobSFErPfR7/VudajK7e0YD1m9bYMpegMtpiTswjEt7/U75EEP6ysG35hDay4RWRrq8qvTjMK3+YRHDMRexL4lvnn4gy5tjKDMSIRxwMfC/FC0Px6wOOIobkNC1WRDYKIZ+C0E2WR4hAnpqMO7gJ4zfaOIYtAxI2uaSMy2E2W2TL0mv5ZgNGTCFJWX0705tPYZY9J8wZdTwFICjH+HGZXJAYJsPyT8jBXizXgcaj2N+EyzPgUdFxNv7NP04JgMT74TDAqnID74mSZae112syIWGF0LtvgsR0LFEt7x19ysuT6QkvPAOd3>
</Properties>
<Symbol>
</Symbol>
<Components>
  <base.V V1 1 360 150 -45 -24 0 3 "1.0" 0 "" 0>
  <AdcDacModule.DAC DAC1 1 610 200 -29 17 0 0 "Analog" 0 "0.0" 0 "0" 0 "LCardParameters" 0>
  <AdcDacModule.ADC ADC1 1 260 200 -19 17 0 0 "Analog" 0 "0.0" 0 "1" 0 "LCardParameters" 0>
  <base.V V2 1 520 150 -45 -24 0 3 "1.0" 0 "" 0>
  <base.DISP DISP1 1 200 410 -37 27 0 0 "1.0" 0 "0.0" 0 "0" 0 "0" 0 "Range:V2=V2,-1,1" 0 "Range:V1=V1,-1,1" 0>
  <Signals.MulConst MulConst1 1 430 200 -19 20 0 0 "0.5" 0>
  <AdcDacModule.LCardParameters LCardParameters 1 470 320 -19 17 0 0 "E154" 0 "5" 0 "[0, 0, 0, 0, 0, 0, 0, 5000, 0] " 0>
  <base.V V3 1 110 310 -45 -24 0 3 "1.0" 0 "" 0>
  <base.Dynamic Dynamic1 1 310 410 -37 28 0 0 "5.0" 0 "Stoermer" 0 "1" 0 "1" 0 "1e-9" 0 "0.02" 0 "1e-11" 0 "1.0" 0 "5" 0 "0.001" 0 "0.01" 0 "0.001" 0 "0.01" 0 "0.001" 0 "0.1" 0 "0" 0 "2" 0 "0" 0 "2" 0 "1" 0 "0" 0 "0" 0 "0.0" 0 "30" 0 "1e10" 0 "0" 0>
  <AdcDacModule.RT RT1 1 210 320 -29 17 0 0 "0" 0>
</Components>
<Wires>
  <360 150 360 200 "" 0 0 0 "">
  <290 200 360 200 "" 0 0 0 "">
  <520 200 580 200 "" 0 0 0 "">
  <520 150 520 200 "" 0 0 0 "">
  <450 200 520 200 "" 0 0 0 "">
  <360 200 410 200 "" 0 0 0 "">
  <110 310 110 320 "" 0 0 0 "">
  <110 320 180 320 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
