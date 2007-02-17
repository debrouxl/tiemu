#!/bin/sh
# This is used on an exploded GDB tarball when doing merges (removes the directories we don't ship).
rm -rf contrib cpu gdb/gdbtk/plugins sim/arm sim/cris sim/d10v sim/erc32 sim/frv sim/h8300 sim/igen sim/iq2000 sim/m32c sim/m32r sim/m68hc11 sim/mcore sim/mips sim/mn10300 sim/ppc sim/sh sim/v850 sim/testsuite/common sim/testsuite/config sim/testsuite/d10v-elf sim/testsuite/frv-elf sim/testsuite/lib sim/testsuite/m32r-elf sim/testsuite/mips64el-elf sim/testsuite/sim md5.sum
