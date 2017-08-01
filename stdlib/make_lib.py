#!/usr/bin/env python
# _*_coding:utf-8_*_

import os

SCM_IN_FILE  = 'lib.scm'
C_OUT_FILE  = 'stdlib.h'

# Read the SCM file
scmFile = open(SCM_IN_FILE, 'rb')
scmSrc = scmFile.read().decode('utf-8')
scmFile.close()

jsSrc = '#define STD_LIB_SRC ' + repr(scmSrc) + ''

# Write the C source
jsFile = open(C_OUT_FILE, 'w')
jsFile.write(jsSrc)
jsFile.close()

