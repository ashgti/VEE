# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'resources/configuration_ui.ui'
#
# Created: Thu Dec  1 11:26:19 2011
#      by: pyside-uic 0.2.13 running on PySide 1.0.8
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(824, 494)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.tabWidget = QtGui.QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName("tabWidget")
        self.tab = QtGui.QWidget()
        self.tab.setObjectName("tab")
        self.gridLayout1 = QtGui.QGridLayout(self.tab)
        self.gridLayout1.setObjectName("gridLayout1")
        self.groupBox = QtGui.QGroupBox(self.tab)
        self.groupBox.setMaximumSize(QtCore.QSize(450, 16777215))
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout.setObjectName("verticalLayout")
        self.browserFiles = QtGui.QPushButton(self.groupBox)
        self.browserFiles.setAutoDefault(False)
        self.browserFiles.setDefault(False)
        self.browserFiles.setFlat(False)
        self.browserFiles.setObjectName("browserFiles")
        self.verticalLayout.addWidget(self.browserFiles)
        self.studentFiles = QtGui.QListWidget(self.groupBox)
        self.studentFiles.setObjectName("studentFiles")
        self.verticalLayout.addWidget(self.studentFiles)
        self.pushButton = QtGui.QPushButton(self.groupBox)
        self.pushButton.setObjectName("pushButton")
        self.verticalLayout.addWidget(self.pushButton)
        self.gridLayout1.addWidget(self.groupBox, 0, 1, 1, 1)
        self.groupBox_6 = QtGui.QGroupBox(self.tab)
        self.groupBox_6.setObjectName("groupBox_6")
        self.verticalLayout_4 = QtGui.QVBoxLayout(self.groupBox_6)
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.buildLog = QtGui.QTextEdit(self.groupBox_6)
        self.buildLog.setObjectName("buildLog")
        self.verticalLayout_4.addWidget(self.buildLog)
        self.gridLayout1.addWidget(self.groupBox_6, 0, 2, 2, 1)
        self.groupBox1 = QtGui.QGroupBox(self.tab)
        self.groupBox1.setObjectName("groupBox1")
        self.horizontalLayout = QtGui.QHBoxLayout(self.groupBox1)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.pushBuild = QtGui.QPushButton(self.groupBox1)
        self.pushBuild.setObjectName("pushBuild")
        self.horizontalLayout.addWidget(self.pushBuild)
        self.pushRun = QtGui.QPushButton(self.groupBox1)
        self.pushRun.setObjectName("pushRun")
        self.horizontalLayout.addWidget(self.pushRun)
        self.pushStop = QtGui.QPushButton(self.groupBox1)
        self.pushStop.setObjectName("pushStop")
        self.horizontalLayout.addWidget(self.pushStop)
        self.gridLayout1.addWidget(self.groupBox1, 1, 1, 1, 1)
        self.tabWidget.addTab(self.tab, "")
        self.tab1 = QtGui.QWidget()
        self.tab1.setObjectName("tab1")
        self.gridLayout_3 = QtGui.QGridLayout(self.tab1)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.groupBox_4 = QtGui.QGroupBox(self.tab1)
        self.groupBox_4.setMaximumSize(QtCore.QSize(200, 16777215))
        self.groupBox_4.setObjectName("groupBox_4")
        self.verticalLayout1 = QtGui.QVBoxLayout(self.groupBox_4)
        self.verticalLayout1.setObjectName("verticalLayout1")
        self.outputPinsList = QtGui.QListWidget(self.groupBox_4)
        self.outputPinsList.setObjectName("outputPinsList")
        self.verticalLayout1.addWidget(self.outputPinsList)
        self.label_2 = QtGui.QLabel(self.groupBox_4)
        self.label_2.setObjectName("label_2")
        self.verticalLayout1.addWidget(self.label_2)
        self.signalName = QtGui.QLineEdit(self.groupBox_4)
        self.signalName.setObjectName("signalName")
        self.verticalLayout1.addWidget(self.signalName)
        self.addSignal = QtGui.QPushButton(self.groupBox_4)
        self.addSignal.setObjectName("addSignal")
        self.verticalLayout1.addWidget(self.addSignal)
        spacerItem = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout1.addItem(spacerItem)
        self.gridLayout_3.addWidget(self.groupBox_4, 0, 0, 1, 1)
        self.groupBox_5 = QtGui.QGroupBox(self.tab1)
        self.groupBox_5.setObjectName("groupBox_5")
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.groupBox_5)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.pinConfiguration = QtGui.QGroupBox(self.groupBox_5)
        self.pinConfiguration.setMinimumSize(QtCore.QSize(0, 0))
        self.pinConfiguration.setCheckable(False)
        self.pinConfiguration.setObjectName("pinConfiguration")
        self.gridLayout_5 = QtGui.QGridLayout(self.pinConfiguration)
        self.gridLayout_5.setObjectName("gridLayout_5")
        self.exponential = QtGui.QRadioButton(self.pinConfiguration)
        self.exponential.setChecked(True)
        self.exponential.setObjectName("exponential")
        self.gridLayout_5.addWidget(self.exponential, 0, 0, 1, 1)
        self.uniform = QtGui.QRadioButton(self.pinConfiguration)
        self.uniform.setObjectName("uniform")
        self.gridLayout_5.addWidget(self.uniform, 2, 0, 1, 1)
        self.expForm = QtGui.QFormLayout()
        self.expForm.setFieldGrowthPolicy(QtGui.QFormLayout.FieldsStayAtSizeHint)
        self.expForm.setObjectName("expForm")
        self.label_13 = QtGui.QLabel(self.pinConfiguration)
        self.label_13.setObjectName("label_13")
        self.expForm.setWidget(0, QtGui.QFormLayout.LabelRole, self.label_13)
        self.expLambda = QtGui.QLineEdit(self.pinConfiguration)
        self.expLambda.setObjectName("expLambda")
        self.expForm.setWidget(0, QtGui.QFormLayout.FieldRole, self.expLambda)
        self.label_14 = QtGui.QLabel(self.pinConfiguration)
        self.label_14.setObjectName("label_14")
        self.expForm.setWidget(1, QtGui.QFormLayout.LabelRole, self.label_14)
        self.expDuration = QtGui.QLineEdit(self.pinConfiguration)
        self.expDuration.setObjectName("expDuration")
        self.expForm.setWidget(1, QtGui.QFormLayout.FieldRole, self.expDuration)
        self.gridLayout_5.addLayout(self.expForm, 1, 0, 1, 1)
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.gridLayout_5.addItem(spacerItem1, 6, 0, 1, 1)
        self.uniForm = QtGui.QFormLayout()
        self.uniForm.setObjectName("uniForm")
        self.aLabel = QtGui.QLabel(self.pinConfiguration)
        self.aLabel.setObjectName("aLabel")
        self.uniForm.setWidget(0, QtGui.QFormLayout.LabelRole, self.aLabel)
        self.uniA = QtGui.QLineEdit(self.pinConfiguration)
        self.uniA.setObjectName("uniA")
        self.uniForm.setWidget(0, QtGui.QFormLayout.FieldRole, self.uniA)
        self.bLabel = QtGui.QLabel(self.pinConfiguration)
        self.bLabel.setObjectName("bLabel")
        self.uniForm.setWidget(1, QtGui.QFormLayout.LabelRole, self.bLabel)
        self.uniB = QtGui.QLineEdit(self.pinConfiguration)
        self.uniB.setObjectName("uniB")
        self.uniForm.setWidget(1, QtGui.QFormLayout.FieldRole, self.uniB)
        self.durationLabel = QtGui.QLabel(self.pinConfiguration)
        self.durationLabel.setObjectName("durationLabel")
        self.uniForm.setWidget(2, QtGui.QFormLayout.LabelRole, self.durationLabel)
        self.uniDuration = QtGui.QLineEdit(self.pinConfiguration)
        self.uniDuration.setObjectName("uniDuration")
        self.uniForm.setWidget(2, QtGui.QFormLayout.FieldRole, self.uniDuration)
        self.gridLayout_5.addLayout(self.uniForm, 3, 0, 1, 1)
        self.deterministic = QtGui.QRadioButton(self.pinConfiguration)
        self.deterministic.setObjectName("deterministic")
        self.gridLayout_5.addWidget(self.deterministic, 4, 0, 1, 1)
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setObjectName("formLayout")
        self.deterministicInterval = QtGui.QLineEdit(self.pinConfiguration)
        self.deterministicInterval.setObjectName("deterministicInterval")
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.deterministicInterval)
        self.label_3 = QtGui.QLabel(self.pinConfiguration)
        self.label_3.setObjectName("label_3")
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.label_3)
        self.deterministicDuration = QtGui.QLineEdit(self.pinConfiguration)
        self.deterministicDuration.setObjectName("deterministicDuration")
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.deterministicDuration)
        self.label_4 = QtGui.QLabel(self.pinConfiguration)
        self.label_4.setObjectName("label_4")
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.label_4)
        self.gridLayout_5.addLayout(self.formLayout, 5, 0, 1, 1)
        self.horizontalLayout_2.addWidget(self.pinConfiguration)
        self.groupBox_2 = QtGui.QGroupBox(self.groupBox_5)
        self.groupBox_2.setObjectName("groupBox_2")
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.formLayout_3 = QtGui.QFormLayout()
        self.formLayout_3.setObjectName("formLayout_3")
        self.serial = QtGui.QRadioButton(self.groupBox_2)
        self.serial.setObjectName("serial")
        self.formLayout_3.setWidget(2, QtGui.QFormLayout.LabelRole, self.serial)
        self.digital = QtGui.QRadioButton(self.groupBox_2)
        self.digital.setChecked(True)
        self.digital.setObjectName("digital")
        self.formLayout_3.setWidget(0, QtGui.QFormLayout.LabelRole, self.digital)
        self.analog = QtGui.QRadioButton(self.groupBox_2)
        self.analog.setObjectName("analog")
        self.formLayout_3.setWidget(1, QtGui.QFormLayout.LabelRole, self.analog)
        self.serialLineEdit = QtGui.QLineEdit(self.groupBox_2)
        self.serialLineEdit.setObjectName("serialLineEdit")
        self.formLayout_3.setWidget(2, QtGui.QFormLayout.FieldRole, self.serialLineEdit)
        self.analogWaveForm = QtGui.QGroupBox(self.groupBox_2)
        self.analogWaveForm.setObjectName("analogWaveForm")
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.analogWaveForm)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.sineWave = QtGui.QRadioButton(self.analogWaveForm)
        self.sineWave.setObjectName("sineWave")
        self.verticalLayout_3.addWidget(self.sineWave)
        self.squareWave = QtGui.QRadioButton(self.analogWaveForm)
        self.squareWave.setObjectName("squareWave")
        self.verticalLayout_3.addWidget(self.squareWave)
        self.label = QtGui.QLabel(self.analogWaveForm)
        self.label.setObjectName("label")
        self.verticalLayout_3.addWidget(self.label)
        self.maxAnalogValue = QtGui.QLineEdit(self.analogWaveForm)
        self.maxAnalogValue.setObjectName("maxAnalogValue")
        self.verticalLayout_3.addWidget(self.maxAnalogValue)
        self.formLayout_3.setWidget(1, QtGui.QFormLayout.FieldRole, self.analogWaveForm)
        self.verticalLayout_2.addLayout(self.formLayout_3)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_2.addItem(spacerItem2)
        self.horizontalLayout_2.addWidget(self.groupBox_2)
        self.gridLayout_3.addWidget(self.groupBox_5, 0, 1, 1, 1)
        self.groupBox_3 = QtGui.QGroupBox(self.tab1)
        self.groupBox_3.setObjectName("groupBox_3")
        self.verticalLayout2 = QtGui.QVBoxLayout(self.groupBox_3)
        self.verticalLayout2.setObjectName("verticalLayout2")
        self.inputPinsList = QtGui.QListWidget(self.groupBox_3)
        self.inputPinsList.setObjectName("inputPinsList")
        self.verticalLayout2.addWidget(self.inputPinsList)
        self.label_21 = QtGui.QLabel(self.groupBox_3)
        self.label_21.setObjectName("label_21")
        self.verticalLayout2.addWidget(self.label_21)
        self.inputSignalName = QtGui.QLineEdit(self.groupBox_3)
        self.inputSignalName.setObjectName("inputSignalName")
        self.verticalLayout2.addWidget(self.inputSignalName)
        self.addInputSignal = QtGui.QPushButton(self.groupBox_3)
        self.addInputSignal.setObjectName("addInputSignal")
        self.verticalLayout2.addWidget(self.addInputSignal)
        spacerItem3 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout2.addItem(spacerItem3)
        self.gridLayout_3.addWidget(self.groupBox_3, 0, 2, 1, 1)
        self.tabWidget.addTab(self.tab1, "")
        self.gridLayout.addWidget(self.tabWidget, 0, 1, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar()
        self.menubar.setGeometry(QtCore.QRect(0, 0, 824, 22))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuScenario_Control = QtGui.QMenu(self.menubar)
        self.menuScenario_Control.setObjectName("menuScenario_Control")
        self.menuWindow = QtGui.QMenu(self.menubar)
        self.menuWindow.setObjectName("menuWindow")
        MainWindow.setMenuBar(self.menubar)
        self.actionNew_Scenario = QtGui.QAction(MainWindow)
        self.actionNew_Scenario.setObjectName("actionNew_Scenario")
        self.actionSave = QtGui.QAction(MainWindow)
        self.actionSave.setObjectName("actionSave")
        self.actionOpen = QtGui.QAction(MainWindow)
        self.actionOpen.setObjectName("actionOpen")
        self.actionRun = QtGui.QAction(MainWindow)
        self.actionRun.setObjectName("actionRun")
        self.actionStop = QtGui.QAction(MainWindow)
        self.actionStop.setObjectName("actionStop")
        self.actionBuild = QtGui.QAction(MainWindow)
        self.actionBuild.setObjectName("actionBuild")
        self.actionSC_View = QtGui.QAction(MainWindow)
        self.actionSC_View.setObjectName("actionSC_View")
        self.actionConfig_View = QtGui.QAction(MainWindow)
        self.actionConfig_View.setObjectName("actionConfig_View")
        self.menuFile.addAction(self.actionNew_Scenario)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionOpen)
        self.menuScenario_Control.addAction(self.actionRun)
        self.menuScenario_Control.addAction(self.actionStop)
        self.menuScenario_Control.addAction(self.actionBuild)
        self.menuWindow.addAction(self.actionSC_View)
        self.menuWindow.addAction(self.actionConfig_View)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuScenario_Control.menuAction())
        self.menubar.addAction(self.menuWindow.menuAction())

        self.retranslateUi(MainWindow)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "Virtual Embedded Enviorment", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("MainWindow", "Runtime Source", None, QtGui.QApplication.UnicodeUTF8))
        self.browserFiles.setText(QtGui.QApplication.translate("MainWindow", "Find Emulator Program", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton.setText(QtGui.QApplication.translate("MainWindow", "Remove From List", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_6.setTitle(QtGui.QApplication.translate("MainWindow", "Build Log", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox1.setTitle(QtGui.QApplication.translate("MainWindow", "Scenario Control", None, QtGui.QApplication.UnicodeUTF8))
        self.pushBuild.setText(QtGui.QApplication.translate("MainWindow", "Build", None, QtGui.QApplication.UnicodeUTF8))
        self.pushRun.setText(QtGui.QApplication.translate("MainWindow", "Run", None, QtGui.QApplication.UnicodeUTF8))
        self.pushStop.setText(QtGui.QApplication.translate("MainWindow", "Stop", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("MainWindow", "Scenaio View", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_4.setTitle(QtGui.QApplication.translate("MainWindow", "Output Signals", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("MainWindow", "Signal Name", None, QtGui.QApplication.UnicodeUTF8))
        self.addSignal.setText(QtGui.QApplication.translate("MainWindow", "Add Signal", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_5.setTitle(QtGui.QApplication.translate("MainWindow", "Signal Configurtion", None, QtGui.QApplication.UnicodeUTF8))
        self.pinConfiguration.setTitle(QtGui.QApplication.translate("MainWindow", "Signal Intervals", None, QtGui.QApplication.UnicodeUTF8))
        self.exponential.setText(QtGui.QApplication.translate("MainWindow", "Exponential", None, QtGui.QApplication.UnicodeUTF8))
        self.uniform.setText(QtGui.QApplication.translate("MainWindow", "Uniform", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("MainWindow", "Lambda", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("MainWindow", "Duration", None, QtGui.QApplication.UnicodeUTF8))
        self.aLabel.setText(QtGui.QApplication.translate("MainWindow", "A", None, QtGui.QApplication.UnicodeUTF8))
        self.bLabel.setText(QtGui.QApplication.translate("MainWindow", "B", None, QtGui.QApplication.UnicodeUTF8))
        self.durationLabel.setText(QtGui.QApplication.translate("MainWindow", "Duration", None, QtGui.QApplication.UnicodeUTF8))
        self.deterministic.setText(QtGui.QApplication.translate("MainWindow", "Deterministic", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("MainWindow", "Interval", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("MainWindow", "Duration", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_2.setTitle(QtGui.QApplication.translate("MainWindow", "Signal Values", None, QtGui.QApplication.UnicodeUTF8))
        self.serial.setText(QtGui.QApplication.translate("MainWindow", "Serial", None, QtGui.QApplication.UnicodeUTF8))
        self.digital.setText(QtGui.QApplication.translate("MainWindow", "Digital", None, QtGui.QApplication.UnicodeUTF8))
        self.analog.setText(QtGui.QApplication.translate("MainWindow", "Analog", None, QtGui.QApplication.UnicodeUTF8))
        self.serialLineEdit.setPlaceholderText(QtGui.QApplication.translate("MainWindow", "Value...", None, QtGui.QApplication.UnicodeUTF8))
        self.analogWaveForm.setTitle(QtGui.QApplication.translate("MainWindow", "Analog Wave Form", None, QtGui.QApplication.UnicodeUTF8))
        self.sineWave.setText(QtGui.QApplication.translate("MainWindow", "Sine Wave", None, QtGui.QApplication.UnicodeUTF8))
        self.squareWave.setText(QtGui.QApplication.translate("MainWindow", "Square Wave", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("MainWindow", "Max Value", None, QtGui.QApplication.UnicodeUTF8))
        self.maxAnalogValue.setPlaceholderText(QtGui.QApplication.translate("MainWindow", "0 to 5v", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_3.setTitle(QtGui.QApplication.translate("MainWindow", "Input", None, QtGui.QApplication.UnicodeUTF8))
        self.label_21.setText(QtGui.QApplication.translate("MainWindow", "Signal Name", None, QtGui.QApplication.UnicodeUTF8))
        self.addInputSignal.setText(QtGui.QApplication.translate("MainWindow", "Add Signal", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab1), QtGui.QApplication.translate("MainWindow", "Configuration", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuScenario_Control.setTitle(QtGui.QApplication.translate("MainWindow", "Scenario Control", None, QtGui.QApplication.UnicodeUTF8))
        self.menuWindow.setTitle(QtGui.QApplication.translate("MainWindow", "Window", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_Scenario.setText(QtGui.QApplication.translate("MainWindow", "&New Scenario", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_Scenario.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setText(QtGui.QApplication.translate("MainWindow", "&Save", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("MainWindow", "&Open", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRun.setText(QtGui.QApplication.translate("MainWindow", "&Run", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRun.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+R", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStop.setText(QtGui.QApplication.translate("MainWindow", "Stop", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBuild.setText(QtGui.QApplication.translate("MainWindow", "Build", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBuild.setToolTip(QtGui.QApplication.translate("MainWindow", "Build", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBuild.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+B", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSC_View.setText(QtGui.QApplication.translate("MainWindow", "Scenario View", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSC_View.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+1", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfig_View.setText(QtGui.QApplication.translate("MainWindow", "Scenario &Configuration", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfig_View.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+2", None, QtGui.QApplication.UnicodeUTF8))

