#! /bin/sh
SRCDIR=$1
PREFIX=$2
EXEC_PREFIX=$3

cp todd $EXEC_PREFIX/bin/todd || exit 1
rm -rf $PREFIX/share/madd.todd
cp -r $SRCDIR/data $PREFIX/share/madd.todd || exit 1

