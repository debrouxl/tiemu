#! /bin/sh
for module in skinedit tiemu; do
echo Updating "$module"
cd "$module/trunk"; rm m4/*.m4; autoreconf -f -i || exit 1; cd ../..
done
