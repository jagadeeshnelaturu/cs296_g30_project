.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
######################################
# Project Name (generate executable with this name)
TARGET = cs296_30_exe

# Project Paths
PROJECT_ROOT=./
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/myobjs
BINDIR = $(PROJECT_ROOT)/mybins
DOCDIR = $(PROJECT_ROOT)/doc
LIBDIR = $(PROJECT_ROOT)/mylibs
EXTER_SRC=$(EXTERNAL_ROOT)/src
FILE_1=./external/include/Box2D/Box2D.h
FILE_2=./external/lib/libBox2D.a

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr/include/

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL
SHARED_LIB =FALSE
STATIC_TARGET= $(LIBDIR)/libCS296test.a
DYNAMIC_TARGET = $(LIBDIR)/libCS296test.so

# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall -fno-strict-aliasing
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib/ -L $(GLUI_ROOT)/lib

######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
FILES_FOR_LIB :=./myobjs/callbacks.o ./myobjs/cs296_base.o ./myobjs/dominos.o ./myobjs/render.o

.PHONY: all setup doc distclean clean report

all: setup exe $(DYNAMIC_TARGET) $(STATIC_TARGET) exelib 

setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mybins
	@mkdir -p mylibs
	@echo "Checking If Box2D Is Installed"
	@if  test -e  $(FILE_1) && test -e $(FILE_2) ; then \
	echo "Box2D is installed!!!!"; \
	else \
	echo "Installing......"; \
	cd $(EXTER_SRC); tar -zxf Box2D.tgz; \
	cd Box2D; \
	mkdir -p build296; \
	cd build296; \
	cmake ../; \
	make; \
	make install; \
	fi 

exe: setup $(OBJS)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $(TARGET))"
	@$(CC) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@mv $(TARGET) ./mybins/$(TARGET)
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include -include $(OBJS:.o=.d)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp 
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) -fPIC $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

$(STATIC_TARGET) :  $(OBJS)
	@if test "$(SHARED_LIB)" = "FALSE"; then \
	echo "Creating Static Library..." ; \
	ar rcs $@ $(FILES_FOR_LIB); \
	fi;

$(DYNAMIC_TARGET) : $(OBJS)
	@if test "$(SHARED_LIB)" = "TRUE"; then \
	echo "Creating Dynamic Library..."; \
	gcc  -fPIC -shared -o $@ $(FILES_FOR_LIB); \
	fi;

exelib: setup $(STATIC_TARGET) $(DYNAMIC_TARGET)
	@$(ECHO) "Creating Executable Using Library..."
	@cd ./myobjs ; \
	$(CC) -fPIC -o cs296_30_exelib $(LDFLAGS) -L ../mylibs/ -L ../external/lib/ -L ../myobjs main.o -lCS296test $(LIBS)
	@mv myobjs/cs296_30_exelib mybins/

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"

clean: 
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR)/* *~ $(DEPS) $(SRCDIR)/*~
	@$(RM) -rf ./mylibs/*
	@$(RM) -rf ./mybins/*
	@$(RM) -rf ./doc/html
	@$(RM) -rf ./doc/cs296_report_30.pdf
	@$(RM) -rf ./doc/cs296_report_30.aux
	@$(RM) -rf ./doc/cs296_report_30.log
	@$(RM) -rf ./doc/cs296_report_30.blg
	@$(RM) -rf ./doc/cs296_report_30.bbl
	@$(ECHO) "Done"

distclean: clean
	@$(RM) -rf $(BINDIR) $(DOCDIR)/html
	@$(RM) -rf ./mybins
	@$(RM) -rf ./mylibs
	@$(RM) -rf ./myobjs
	@$(RM) -rf ./external/src/Box2D
	@$(RM) -rf ./external/include/*
	@$(RM) -rf ./external/lib/*

report: 
	@ cd ./doc && pdflatex cs296_report_30.tex && bibtex cs296_report_30.aux && pdflatex cs296_report_30.tex && pdflatex cs296_report_30.tex
