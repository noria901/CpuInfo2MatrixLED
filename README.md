# CpuInfo2MatrixLED

Display the CPU status of the host PC on the matrix LED

#Arduino
*Arduino Pro Micro 5V 16MHz
  *https://www.google.co.jp/search?q=aliexpress+pro+micro&oq=aliexpress+pro+micro&aqs=chrome..69i57j0l2j69i64.9253j0j7&client=ubuntu&sourceid=chrome&ie=UTF-8
*MAX7219 MatrixLED driver Kit
  *http://www.aitendo.com/product/11714

#Run on HostPC
*Install Python modules.  
`$ ./install.sh`
`$ dmesg
[12143.504603] cdc_acm 3-3.4:1.0: ttyACM0: USB ACM device`
  *In this case / dev / ttyACM 0 is Arduino.
  *Update the definition of cpuinfo2matrixled.sh accordingly
  *
*Run  
`$ ./cpuinfo2matrixled.sh`
