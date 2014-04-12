OBJDIR := build

VPATH = examples

all: lib examples

%: %.o
	g++ $(CPPFLAGS) $(LDFLAGS) -o $@ $^

lib: $(OBJDIR)/usbiss.o

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	g++ -c $(CPPFLAGS) $(LDFLAGS) -o $@ $^

examples: $(OBJDIR)/leds $(OBJDIR)/modes_and_analog

$(OBJDIR)/leds: $(OBJDIR)/leds.o $(OBJDIR)/usbiss.o
$(OBJDIR)/modes_and_analog: $(OBJDIR)/modes_and_analog.o $(OBJDIR)/usbiss.o

clean:
	rm -rf build
