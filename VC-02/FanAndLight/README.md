# Project Build Guide (Docker-based)

This project uses **Docker** to simplify environment setup and ensure build consistency.

---

## Requirements

- Docker installed on the host machine
- Linux host (tested on Ubuntu)

---

## Docker Setup

### 1. Pull and Run Ubuntu 18.04 Container

```bash
docker pull ubuntu:18.04

docker run -it \
  --name ubuntu18_vc02 \
  -v /home/user/docker_root:/data \
  ubuntu:18.04 /bin/bash
```

### 2. Create a Development User Inside the Container

```bash
apt update
apt install -y sudo

useradd -m -u 1000 -s /bin/bash dev
passwd dev
usermod -aG sudo dev
exit
```

---

## Using the Docker Container

### Start and Enter the Container

Run these commands each time you want to work:

```bash
docker start ubuntu18_vc02
docker exec -it --user dev ubuntu18_vc02 /bin/bash
```

---

## Install Required Tools (Inside Docker)

```bash
sudo apt-get update

sudo apt-get install -y \
  python \
  lame \
  git \
  lib32stdc++6 \
  lib32z1 \
  lib32ncurses5 \
  libbz2-1.0:i386
```

### Clone Andestech SDK

```bash
sudo git clone https://gitee.com/seahi007/andestech.git /opt/Andestech
```

---

## Modify Source Code

Edit the following file as needed:

```
unione_lite_app_hb_m/user/src/examples/hb_auto_gpio.c
```

---

## Build the Project

Run the build script:

```bash
./build.sh update
```

### Output File

After a successful build, the output binary will be located at:

```
uni_hb_m_solution/unione_lite_app_hb_m/output/uni_app_debug_update.bin
```

---

## Flashing the Firmware

Flash the generated binary using the **UART Burning Tool**:

- Download tool:  
  https://docs.ai-thinker.com/_media/hummingbird-m-update-tool.zip

- Flash file:  
  ```
  uni_app_debug_update.bin
  ```

---

## Notes

- Make sure the UART connection is properly configured before flashing.
- Always rebuild after modifying source files.