DIR_SRC = ./src

AM_CFLAGS = -Wall \
    -Wundef \
    -Wno-trigraphs \
    -g -O0 \
    -Dstrlcpy=g_strlcpy \
    -Dstrlcat=g_strlcat \
    -Werror=format-security \
    $(QMIFRAMEWORK_CFLAGS) \
    $(QMI_CFLAGS) \
    -DFEATURE_DSS_LINUX \
    -I$(WORKSPACE)/diag/include \
    -I$(WORKSPACE)/common/inc \
    -I./inc \
    -I./sdk-includes/ \
    -I./sdk-includes/dsutils \
    -I./sdk-includes/dsi \
    -I./sdk-includes/data \
    -I./sdk-includes/common \
    -I./sdk-includes/gps \
    -I./sdk-includes/json-c \
    -I./sdk-includes/mqtt \
    -I./sdk-includes/curl \
    -I./sdk-includes/openssl
    
ACLOCAL_AMFLAGS = -I m4

SRCS = \
      $(DIR_SRC)/main.c \
      $(DIR_SRC)/OTAUpgrade.c   \
      $(DIR_SRC)/BTControl.c  \
      $(DIR_SRC)/LedControl.c  \
      $(DIR_SRC)/GpioControl.c  \
      $(DIR_SRC)/uart_debug.c \
      $(DIR_SRC)/GPSControl.c \
      $(DIR_SRC)/EthControl.c \
      $(DIR_SRC)/FTPControl.c \
      $(DIR_SRC)/LinuxTimer.c \
      $(DIR_SRC)/uartapp.c \
      $(DIR_SRC)/spidev_test.c \
      $(DIR_SRC)/TCPDemo.c \
      $(DIR_SRC)/ADC.c \
      $(DIR_SRC)/I2C.c \
      $(DIR_SRC)/demoVersion.c \
      $(DIR_SRC)/json-c.c \
      $(DIR_SRC)/http_s.c \
      $(DIR_SRC)/mqtt_s.c \
      $(DIR_SRC)/ftp_s.c \
      $(DIR_SRC)/openssl.c \
      $(DIR_SRC)/simcom_lbs_test.c \
      $(DIR_SRC)/simcom_common.c \
      $(DIR_SRC)/simcom_test_main.c

bin_PROGRAMS = simcom_demo
simcom_demo_CC = @CC@
pkgconfigdir = $(libdir)/pkgconfig
simcom_demo_SOURCES = $(SRCS)

if USE_GLIB
simcom_demo_CFLAGS = -DUSE_GLIB $(AM_CFLAGS) -fPIC  @GLIB_CFLAGS@
simcom_demo_CPPFLAGS = $(AM_CPPFLAGS) -DUSE_GLIB  @GLIB_CFLAGS@
else
simcom_demo_CFLAGS = $(AM_CFLAGS) -fPIC
simcom_demo_CPPFLAGS = $(AM_CPPFLAGS)
endif

simcom_demo_LDFLAGS = -lpthread -lrt  -ldl -lm $(QMIFRAMEWORK_LIBS) $(QMI_LIBS) $(DSUTILS_LIBS)
simcom_demo_LDFLAGS += -l$(WORKSPACE)/poky/build/tmp-glibc/work/armv7at2hf-neon-oe-linux-gnueabi/data/git-r26/image/usr/lib/libdsi_netctrl.so
simcom_demo_LDFLAGS += -l$(WORKSPACE)/poky/build/tmp-glibc/work/armv7at2hf-neon-oe-linux-gnueabi/curl/7.61.0-r0/image/usr/lib/libcurl.so
simcom_demo_LDFLAGS += -l$(WORKSPACE)/poky/build/tmp-glibc/work/armv7at2hf-neon-oe-linux-gnueabi/openssl/1.1.1b-r0/image/usr/lib/libssl.so
simcom_demo_LDFLAGS += -l$(WORKSPACE)/poky/build/tmp-glibc/work/armv7at2hf-neon-oe-linux-gnueabi/openssl/1.1.1b-r0/image/usr/lib/libcrypto.so

simcom_demo_LDADD = libs/libsdk.a
