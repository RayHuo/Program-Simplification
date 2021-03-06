#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/sources/Consequence.o \
	${OBJECTDIR}/sources/DLP.o \
	${OBJECTDIR}/sources/DependenceGraph.o \
	${OBJECTDIR}/sources/GRSDLP.o \
	${OBJECTDIR}/sources/GRSNLP.o \
	${OBJECTDIR}/sources/Loop.o \
	${OBJECTDIR}/sources/MaxU.o \
	${OBJECTDIR}/sources/NLP.o \
	${OBJECTDIR}/sources/Rule.o \
	${OBJECTDIR}/sources/Utils.o \
	${OBJECTDIR}/sources/Vocabulary.o \
	${OBJECTDIR}/sources/global.o \
	${OBJECTDIR}/sources/lex.o \
	${OBJECTDIR}/sources/main.o \
	${OBJECTDIR}/sources/parse.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/programsimplification

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/programsimplification: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/programsimplification ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/sources/Consequence.o: sources/Consequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/Consequence.o sources/Consequence.cpp

${OBJECTDIR}/sources/DLP.o: sources/DLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/DLP.o sources/DLP.cpp

${OBJECTDIR}/sources/DependenceGraph.o: sources/DependenceGraph.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/DependenceGraph.o sources/DependenceGraph.cpp

${OBJECTDIR}/sources/GRSDLP.o: sources/GRSDLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/GRSDLP.o sources/GRSDLP.cpp

${OBJECTDIR}/sources/GRSNLP.o: sources/GRSNLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/GRSNLP.o sources/GRSNLP.cpp

${OBJECTDIR}/sources/Loop.o: sources/Loop.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/Loop.o sources/Loop.cpp

${OBJECTDIR}/sources/MaxU.o: sources/MaxU.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/MaxU.o sources/MaxU.cpp

${OBJECTDIR}/sources/NLP.o: sources/NLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/NLP.o sources/NLP.cpp

${OBJECTDIR}/sources/Rule.o: sources/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/Rule.o sources/Rule.cpp

${OBJECTDIR}/sources/Utils.o: sources/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/Utils.o sources/Utils.cpp

${OBJECTDIR}/sources/Vocabulary.o: sources/Vocabulary.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/Vocabulary.o sources/Vocabulary.cpp

${OBJECTDIR}/sources/global.o: sources/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/global.o sources/global.cpp

${OBJECTDIR}/sources/lex.o: sources/lex.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/lex.o sources/lex.cpp

${OBJECTDIR}/sources/main.o: sources/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/main.o sources/main.cpp

${OBJECTDIR}/sources/parse.o: sources/parse.cpp 
	${MKDIR} -p ${OBJECTDIR}/sources
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sources/parse.o sources/parse.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/programsimplification

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
