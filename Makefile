CC = i686-elf-gcc
CXX = i686-elf-g++
AS = i686-elf-as
LD = i686-elf-gcc

CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -Iinclude
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fno-use-cxa-atexit
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

SRCDIR = src
BUILDDIR = build
ISODIR = iso

CPP_SOURCES = $(shell find $(SRCDIR) -name '*.cpp')
C_SOURCES = $(shell find $(SRCDIR) -name '*.c')
ASM_SOURCES = $(shell find $(SRCDIR) -name '*.s')

CPP_OBJECTS = $(CPP_SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
C_OBJECTS = $(C_SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
ASM_OBJECTS = $(ASM_SOURCES:$(SRCDIR)/%.s=$(BUILDDIR)/%.o)

ALL_OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS)

all: myos.iso

myos.bin: $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

myos.iso: myos.bin
	mkdir -p $(ISODIR)/boot/grub
	cp myos.bin $(ISODIR)/boot/
	cp grub.cfg $(ISODIR)/boot/grub/
	grub-mkrescue -o $@ $(ISODIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

clean:
	rm -rf $(BUILDDIR) $(ISODIR) myos.bin myos.iso

test: myos.iso
	qemu-system-i386 -cdrom myos.iso -m 512M

.PHONY: all clean test
