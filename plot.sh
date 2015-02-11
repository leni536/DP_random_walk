#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace
set -o pipefail

tmpdir=`mktemp -d`

if [ $# -eq 1 ]; then
	fname="$1"
	cat "${fname}" | sed -e '/^#/d' > "${tmpdir}/fifo" 
else
	cat | sed -e '/^#/d' > "${tmpdir}/fifo" 
fi

gnuplot << EOD
plot '${tmpdir}/fifo'
pause mouse button1
EOD

rm -rf "${tmpdir}"
