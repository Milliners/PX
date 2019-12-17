PROTO_CSRCS = \
	main.c 
HSRCS	= \
	headers.h

CSRCS	= ${PROTO_CSRCS} 

NAME	= bootloader

CPROTO 	= cproto

OUT	= out

BIN_DIR = ${OUT}/bin
DEP_DIR = ${OUT}/.deps

$(shell mkdir -p ${BIN_DIR})
$(shell mkdir -p ${DEP_DIR})

TARGET = ${BIN_DIR}/${NAME}.efi

default: ${TARGET}

ASTYLE		= astyle
ASTYLE_OPTS 	= -A3 -s2 --attach-extern-c -L -c -w -Y -m0 -f -p  -H -U -k3 -xj -xd

DEPFLAGS	= -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
OBJS		= ${CSRCS:%.c=${BIN_DIR}/%.o}

GNUEFI_PREFIX	= /usr
EFI_INC		= ${GNUEFI_PREFIX}/include/efi
EFI_ARCH	= x86_64
EFI_ARCH_DIR	= x86_64

ifeq (,$(wildcard ${GNUEFI_PREFIX}/lib/crt0-efi-${EFI_ARCH_DIR}.o))
  #redhat
  EFI_LIB	= ${GNUEFI_PREFIX}/lib64/gnuefi
  LIB		= /usr/lib64
else
  # debian
  EFI_LIB	= ${GNUEFI_PREFIX}/lib
  LIB		= /usr/lib/x86_64-linux-gnu
endif

GCC_ARCH	= -m64

OPT 		= -O2

EFI_CRT_OBJS	= $(EFI_LIB)/crt0-efi-$(EFI_ARCH_DIR).o
EFI_LDS		= $(EFI_LIB)/elf_$(EFI_ARCH_DIR)_efi.lds

CPPFLAGS	= -I.  -I$(EFI_INC) -I$(EFI_INC)/$(EFI_ARCH_DIR) -I$(EFI_INC)/protocol 

CFLAGS		= ${DEPFLAGS} ${GCC_ARCH} ${CPU_ARCH_FLAGS} ${OPT} \
			-fno-stack-protector -fpic \
		  	-fshort-wchar -mno-red-zone -fno-strict-aliasing \
		  	-fno-merge-constants -Wall -Wno-unused -Werror \
		  	-fno-omit-frame-pointer

CFLAGS 		+= -DEFI_FUNCTION_WRAPPER

GCCLIB		=  $(shell $(CC) ${CFLAGS} ${CPPFLAGS} -print-libgcc-file-name)

LDFLAGS		= -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -Bsymbolic -L $(EFI_LIB) -L $(LIB) 

LIBS		= -lefi -lgnuefi $(GCCLIB) 

${BIN_DIR}/%.o:%.c
${BIN_DIR}/%.o:%.c ${DEP_DIR}/%.d
	${CC} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<
	mv -f ${DEP_DIR}/$*.Td ${DEP_DIR}/$*.d

$(DEP_DIR)/%.d: ;
.PRECIOUS: $(DEP_DIR)/%.d

${BIN_DIR}/${PROG}_${EFI_ARCH}.efi:${BUILD_DIR}/${PROG}.efi
	cp $< $@ 

${BIN_DIR}/${NAME}.so: $(OBJS) 
	ld $(LDFLAGS) ${EFI_CRT_OBJS} $(OBJS) -o $@ ${LIBS} 

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(EFI_ARCH) $^ $@
	strip $@

-include $(patsubst %,$(DEP_DIR)/%.d,$(basename $(CSRCS)))

clean:
	rm -rf ${OUT}
	rm -f *% *~  *.orig

protos:
	echo -n > prototypes.h
	${CPROTO} ${CPPFLAGS} -v -e ${PROTO_CSRCS} > prototypes.h.tmp
	mv -f prototypes.h.tmp prototypes.h
	${ASTYLE} ${ASTLYE_OPTS} prototypes.h


tidy:
	${ASTYLE} ${ASTLYE_OPTS} ${PROTO_CSRCS} ${HSRCS}
