#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace
set -o pipefail

GIT_STATUS=`git status --porcelain`
MACRO_DEF='#define VERSION'

if echo "${GIT_STATUS}" | cut -b 4- | grep -q -v '^\(create_version\.sh\|\.gitignore\|\)$'; then
	echo "${MACRO_DEF} \"unclean\"" > include/version.h;
else
	COMMIT_HASH=`git rev-parse --verify HEAD`
	echo "${MACRO_DEF} \"commit_${COMMIT_HASH}\"" > include/version.h;
fi

