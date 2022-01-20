#!/bin/bash
set -e

[ $1 ] && COMMIT_ID=$1 || COMMIT_ID=master
[ $2 ] && DIR=$2 || DIR=openwrt-build

rm -rf ${DIR}
mkdir -p ${DIR}
cd ${DIR}

git init .
git remote add origin https://github.com/openwrt/openwrt.git

if [ "${COMMIT_ID}" = "master" ]; then
	# master branch
	echo "default master branch..."
	git -c protocol.version=2 fetch --no-tags --prune --progress --no-recurse-submodules --depth=1 origin ${COMMIT_ID}
	git checkout --progress --force -B build refs/remotes/origin/master
else
	# use commit id
	echo "commit-id: ${COMMIT_ID}"
	git -c protocol.version=2 fetch --no-tags --prune --progress --no-recurse-submodules --depth=1 origin +${COMMIT_ID}:refs/remotes/origin/master
	git checkout --progress --force -B build ${COMMIT_ID}
fi
git log -1 --format='%H'
exit $?

