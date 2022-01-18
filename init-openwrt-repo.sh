#!/bin/bash
DIR=openwrt
rm -rf ${DIR}
mkdir -p ${DIR}
cd ${DIR}
git init .
git remote add origin https://github.com/openwrt/openwrt.git
git fetch --no-tags --prune --progress --no-recurse-submodules --depth=1 origin master
git checkout --progress --force -B master refs/remotes/origin/master
