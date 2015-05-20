All code developed for the gateway can be found in under "cat", while all other
folders contain third-party libraries.

Requirements
==
The gateway is design to run on the Raspberry Pi platform with the use of a 
nRF24L01+ radio module.

Library Compilation
==
- All RF24 libraries should be installed by running "make" and "make install"
in each root directory.
- tiny-AES128-C is compiled automatically with the gateway.
- The following libraries need to be installed separately
  (it is highly recommended that the Debian APT tool is used to achieve this).
  -- GNU Multiprecision
  -- SQLite3
  -- libcurl
  -- GNU libmicrohttpd

Gateway Compilation
==
Once the above libraries have been installed, the gateway can be compiled by simply
running "make" in the src directory. This should generate the executable file 
"sensor-gateway".

Test Compilation
==
The compilation of unit tests requires the Boost Unit Test Framework library.
Once this is installed, simply run "make" in the src/cat/tests/snsrgtwy folder,
which should generate the executable file "test-snsrgtwy". Note that some tests
will most likely fail due to the lack of a dummy web server to emulate EMonCMS.
