#!/bin/sh

printf "[INFO] Remove '/opt/intel/common/mdf/lib64/igfxcmrt64.so'\n"
sudo rm -r /opt/intel/common/mdf/lib64/igfxcmrt64.so

printf "[INFO] Make soft links\n"
sudo ln -sf /opt/intel/mediasdk/lib64/libmfx.so.1.28 /opt/intel/mediasdk/lib64/libmfx.so.1
sudo ln -sf /opt/intel/mediasdk/lib64/libigdgmm.so.1.0.0 /opt/intel/mediasdk/lib64/libigdgmm.so.1
sudo ln -sf /opt/intel/mediasdk/lib64/libmfxhw64.so.1.28 /opt/intel/mediasdk/lib64/libmfxhw64.so.1
sudo ln -sf /opt/intel/mediasdk/lib64/libva.so.2.300.0 /opt/intel/mediasdk/lib64/libva.so.2
sudo ln -sf /opt/intel/mediasdk/lib64/libva-drm.so.2.300.0 /opt/intel/mediasdk/lib64/libva-drm.so.2
sudo ln -sf /opt/intel/mediasdk/lib64/libva-glx.so.2.300.0 /opt/intel/mediasdk/lib64/libva-glx.so.2
sudo ln -sf /opt/intel/mediasdk/lib64/libva-x11.so.2.300.0 /opt/intel/mediasdk/lib64/libva-x11.so.2
