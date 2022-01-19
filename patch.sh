#!/bin/bash
set -e

BOARD=$1
PATCH_PATH=../patch/common

case "$1" in
	x86_64|hlk-7621a)
	PATCH_PATH=`echo ${PATCH_PATH} ../patch/$1`
	;;
	*)
	echo "invalid board: ${BOARD}"
	exit 1
	;;
esac

for BOARD_PATCH_PATH in ${PATCH_PATH}; do
	if [ -d ${BOARD_PATCH_PATH} ]; then
		echo "patch ${BOARD_PATCH_PATH}"
		for PATCH_F in ${BOARD_PATCH_PATH}/*.patch; do
			[ -f ${PATCH_F} ] && patch -p1 -f < ${PATCH_F}
		done
	fi
done

set +e
echo "update config..."
[ -f ../${BOARD}.config ] && cat ../${BOARD}.config && cp -afv ../${BOARD}.config .config
exit $?

