#!/bin/bash

./ekoder ./izvoriste_1.txt ./ekoderOut.txt ./ekoderCode.txt
./zkoder ./ekoderOut.txt ./zkoderOut.txt
./kanal ./zkoderOut.txt ./kanalOut.txt
./zdekoder ./kanalOut.txt ./zdekoderOut.txt
./edekoder ./zdekoderOut.txt ./ekoderCode.txt ./odrediste_1.txt
