#!/bin/bash

[ -f ../$1.config ] && cat ../$1.config && cp -afv ../$1.config .config
case "$1" in
    x86_64)
    patch p1 < ../99-default_network.patch
	;;
    hilink-mt7621at)
    cp -afv ../610-v5.13-58-net-ethernet-mtk_eth_soc-add-ipv6-flow-offloading-support.patch target/linux/generic/backport-5.10/
    ;;
esac
