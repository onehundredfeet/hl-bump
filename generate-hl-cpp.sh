#!/bin/bash
haxe -cp generator  -lib hl-idl --macro "BumpGenerator.generateCpp()"