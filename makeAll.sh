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

cd ../../mgl_audio/src
make
make static

echo "done"
