#!/bin/bash

echo "building base MoGUL libs..."
cd mgl_types/src
make
make static

cd ../../mgl_logger/src
make
make static

cd ../../mgl_resource/src
make
make static

cd ../../mgl_config/src
make
make static

echo "building core MoGUL libs..."

cd ../../mgl_graphics/src
make
make static

echo "building audio MoGUL lib..."

cd ../../mgl_audio/src
make
make static

echo "building input MoGUL lib..."

cd ../../mgl_input/src
make
make static

echo "building level MoGUL lib..."

cd ../../mgl_level/src
make
make static

echo "done"
