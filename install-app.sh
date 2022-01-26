#!/bin/bash

APP_PATH=../app
if [ -d ${APP_PATH} ]; then
	for APP in ${APP_PATH}/*; do
		[ -d ${APP} -a -f ${APP}/Makefile ] && ln -sfv ../${APP} ./package/$(basename ${APP}) || echo "${APP} [skipped]"
	done
fi
