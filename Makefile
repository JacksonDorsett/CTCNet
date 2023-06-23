CC=gcc                     # The compiler being used
CFLAGS=-Iinclude -Wall     # Compiler flags, -I adds include directory
SRCDIR=src
BUILDDIR=build
BINDIR=bin
TARGET=$(BINDIR)/yourprogram

# The final binary to build
$(TARGET): $(BUILDDIR)/main.o $(BUILDDIR)/other.o
	$(CC) $(BUILDDIR)/main.o $(BUILDDIR)/other.o -o $(TARGET)

# Build the main .o file
$(BUILDDIR)/main.o: $(SRCDIR)/main.c
	$(CC) -c $(CFLAGS) $< -o $@

# Build the other .o file
$(BUILDDIR)/other.o: $(SRCDIR)/other.c
	$(CC) -c $(CFLAGS) $< -o $@

# Clean build
.PHONY: clean
clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)
