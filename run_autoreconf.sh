#! /bin/sh
for module in skinedit tiemu; do
echo Updating "$module"
cd "$module/trunk"; autoreconf -f -i || exit 1; cd ../..
done
