#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f
MV=mv
CP=cp

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=${DISTDIR}/new-rotlib.X.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=${DISTDIR}/new-rotlib.X.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../global/registers.c picamp-rotary.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1158682926/registers.p1 ${OBJECTDIR}/new-rotlib.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1158682926/registers.p1.d ${OBJECTDIR}/new-rotlib.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1158682926/registers.p1 ${OBJECTDIR}/new-rotlib.p1

# Source Files
SOURCEFILES=../global/registers.c picamp-rotary.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/new-rotlib.X.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F887
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1158682926/registers.p1: ../global/registers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1158682926"
	@${RM} ${OBJECTDIR}/_ext/1158682926/registers.p1.d
	@${RM} ${OBJECTDIR}/_ext/1158682926/registers.p1
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1158682926/registers.p1 ../global/registers.c
	@-${MV} ${OBJECTDIR}/_ext/1158682926/registers.d ${OBJECTDIR}/_ext/1158682926/registers.p1.d
	@${FIXDEPS} ${OBJECTDIR}/_ext/1158682926/registers.p1.d $(SILENT) -rsi ${MP_CC_DIR}../

${OBJECTDIR}/new-rotlib.p1: picamp-rotary.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}"
	@${RM} ${OBJECTDIR}/new-rotlib.p1.d
	@${RM} ${OBJECTDIR}/new-rotlib.p1
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/new-rotlib.p1 picamp-rotary.c
	@-${MV} ${OBJECTDIR}/new-rotlib.d ${OBJECTDIR}/new-rotlib.p1.d
	@${FIXDEPS} ${OBJECTDIR}/new-rotlib.p1.d $(SILENT) -rsi ${MP_CC_DIR}../

else
${OBJECTDIR}/_ext/1158682926/registers.p1: ../global/registers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1158682926"
	@${RM} ${OBJECTDIR}/_ext/1158682926/registers.p1.d
	@${RM} ${OBJECTDIR}/_ext/1158682926/registers.p1
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1158682926/registers.p1 ../global/registers.c
	@-${MV} ${OBJECTDIR}/_ext/1158682926/registers.d ${OBJECTDIR}/_ext/1158682926/registers.p1.d
	@${FIXDEPS} ${OBJECTDIR}/_ext/1158682926/registers.p1.d $(SILENT) -rsi ${MP_CC_DIR}../

${OBJECTDIR}/new-rotlib.p1: picamp-rotary.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}"
	@${RM} ${OBJECTDIR}/new-rotlib.p1.d
	@${RM} ${OBJECTDIR}/new-rotlib.p1
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/new-rotlib.p1 picamp-rotary.c
	@-${MV} ${OBJECTDIR}/new-rotlib.d ${OBJECTDIR}/new-rotlib.p1.d
	@${FIXDEPS} ${OBJECTDIR}/new-rotlib.p1.d $(SILENT) -rsi ${MP_CC_DIR}../

endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/new-rotlib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${DISTDIR}
	${MP_AR} $(MP_EXTRA_AR_PRE) --target $(MP_PROCESSOR_OPTION)     -r  ${DISTDIR}/new-rotlib.X.a  ${OBJECTFILES_QUOTED_IF_SPACED}
	@${RM} ${DISTDIR}/new-rotlib.X.hex

else
${DISTDIR}/new-rotlib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${DISTDIR}
	${MP_AR} $(MP_EXTRA_AR_PRE) --target $(MP_PROCESSOR_OPTION)  -r  ${DISTDIR}/new-rotlib.X.a  ${OBJECTFILES_QUOTED_IF_SPACED}

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
