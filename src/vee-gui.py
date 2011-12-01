#!/usr/bin/env python
# Copyright John Harrison, 2011
##
#  MainWindow is the driver program for the GUI components of VEE.

from __future__ import print_function

import sys, os
import os.path
import pickle
import tempfile
from subprocess import Popen, PIPE

try:
    from PySide import QtCore, QtGui
except ImportError as e:
    ## Added to fix dynlib bug
    sys.path.insert(0, os.path.join(os.getcwd(), 'lib', 'python2.7','lib-dynload'))
    from PySide import QtCore, QtGui
from vee.configuration_ui import Ui_MainWindow as CWindow

SRC_DIR = '/Users/john/Projects/VEE'

## ApplicationRunner, an abstract way of running commands, such as make.
class ApplicationRunner(object):
    ## Initialize the ApplicationRunner
    def __init__(self, dest):
        print(dest)
        self.scenario_main = dest
        os.chdir(SRC_DIR)

    ## Build the students work.
    def make(self):
        """
        Operations:
          1. Copy Files to a new location.
          2. Extract Files if compressed.
          3. Compile against students code.
        """
        os.chdir(SRC_DIR)
        os.system('make')
        # while make.poll() == False:
        #     print make.communicate()[0]

    ## Run the student's program with a given scenario
    def run(self):
        """
        Run the scenario with the current configuration files.
        """
        os.chdir(SRC_DIR)
        os.system('python ./src/vee-cmd.py /Users/john/Projects/VEE/test/test.cfg')
        # while run.poll() == False:
        #     print run.communicate()[0]

    def results(self):
        """
        Return the results of the run.
        """
        pass

class ConfigurationError(ValueError):
    pass

## Main application window.
class MainWindow(QtGui.QMainWindow):
    list_changed = QtCore.Signal()

    def __init__(self, ui):
        "Initialize the Window, connects the UI components and initializes variables."
        super(MainWindow, self).__init__()
        self.ui = ui
        self.settings = {}
        self.ui.setupUi(self)
        self.connectComponents()
        self.ui.outputPinsList.setCurrentRow(0)

    def connectComponents(self):
        "Connects the various UI components to python functions."
        ui = self.ui
        ui.outputPinsList.currentItemChanged\
                .connect(self.onOutputPinsListSelect)

        # Menu Actions
        ui.actionNew_Scenario.triggered.connect(self.resetData)
        ui.actionSave.triggered.connect(self.saveScenario)
        ui.actionRun.triggered.connect(self.runScenario)
        ui.actionOpen.triggered.connect(self.openScenario)

        # Add Validators
        ui.uniA.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uniA))
        ui.uniB.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uniB))
        ui.uniDuration.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uniDuration))

        ui.expLambda.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.expLambda))
        ui.expDuration.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.expDuration))

        # Connect behaviors
        ui.uniA.textEdited.connect(self.updatePinConfiguration)
        ui.uniB.textEdited.connect(self.updatePinConfiguration)
        ui.uniDuration.textEdited.connect(self.updatePinConfiguration)

        ui.expLambda.textEdited.connect(self.updatePinConfiguration)
        ui.expDuration.textEdited.connect(self.updatePinConfiguration)

        ui.uniform.clicked.connect(self.updatePinConfiguration)
        ui.exponential.clicked.connect(self.updatePinConfiguration)
        ui.analog.clicked.connect(self.updatePinConfiguration)
        ui.digital.clicked.connect(self.updatePinConfiguration)
        ui.serial.clicked.connect(self.updatePinConfiguration)

        ui.sineWave.clicked.connect(self.updatePinConfiguration)
        ui.squareWave.clicked.connect(self.updatePinConfiguration)
        ui.maxAnalogValue.textEdited.connect(self.updatePinConfiguration)
        ui.serialLineEdit.textEdited.connect(self.updatePinConfiguration)

        # Button Behaviors
        ui.browserFiles.clicked.connect(self.browse)

        ui.addSignal.clicked.connect(self.addSignal)
        ui.pushRun.clicked.connect(self.runScenario)

        # A useful default. Should not exist in production.
        if os.path.isfile("/Users/john/Projects/VEE/student.cc"):
            self.ui.studentFiles.addItem("/Users/john/Projects/VEE/student.cc")

        # Default Configurations
        ui.exponential.setEnabled(False)
        ui.expLambda.setEnabled(False)
        ui.expDuration.setEnabled(False)
        ui.uniform.setEnabled(False)
        ui.uniA.setEnabled(False)
        ui.uniB.setEnabled(False)
        ui.uniDuration.setEnabled(False)
        ui.deterministic.setEnabled(False)
        ui.deterministicInterval.setEnabled(False)
        ui.deterministicDuration.setEnabled(False)

        ui.digital.setEnabled(False)
        ui.analog.setEnabled(False)
        ui.serial.setEnabled(False)
        ui.sineWave.setEnabled(False)
        ui.squareWave.setEnabled(False)
        ui.maxAnalogValue.setEnabled(False)
        ui.serialLineEdit.setEnabled(False)

    def addSignal(self):
        """Add a signal to the list of signals."""
        name = self.ui.signalName.text()
        if name:
            self.ui.outputPinsList.addItem(name)
            self.ui.signalName.clear()
        else:
            return

    def getCurrentSignalId(self):
        """Get the name of the current Signal"""
        current = self.ui.outputPinsList.currentItem()
        if current:
            return current.text()
        else:
            None

    def runScenario(self):
        scenario_file = self.ui.studentFiles.currentItem()
        print(scenario_file.text())
        a = ApplicationRunner(scenario_file.text())
        tmp_file = tempfile.NamedTemporaryFile()
        self._saveAs(tmp_file)
        print(tmp_file.name)
        r = a.make()
        print(r)
        r = a.run()
        print(r)

    def updatePinConfiguration(self):
        "Update the configuration settings."
        pinData = { }

        if self.getCurrentSignalId() == None:
            return

        if self.ui.exponential.isChecked():
            self.ui.uniA.setEnabled(False)
            self.ui.uniB.setEnabled(False)
            self.ui.uniDuration.setEnabled(False)
            self.ui.expLambda.setEnabled(True)
            self.ui.expDuration.setEnabled(True)
            pinData['signalType'] = 'exp'
            pinData['expLambda'] = self.ui.expLambda.text()
            pinData['expDuration'] = self.ui.expDuration.text()
        elif self.ui.uniform.isChecked():
            self.ui.expLambda.setEnabled(False)
            self.ui.expDuration.setEnabled(False)
            self.ui.uniA.setEnabled(True)
            self.ui.uniB.setEnabled(True)
            self.ui.uniDuration.setEnabled(True)
            pinData['signalType'] = 'uni'
            pinData['uniA'] = self.ui.uniA.text()
            pinData['uniB'] = self.ui.uniB.text()
            pinData['uniDuration'] = self.ui.uniDuration.text()
        else:
            raise ConfigurationError("Unknown pin type.")

        if self.ui.analog.isChecked():
            pinData['dataType'] = 'analog'
            pinData['maxAnalogValue'] = self.ui.maxAnalogValue.text()
            if self.ui.sineWave.isChecked():
                pinData['waveForm'] = 'sine'
            elif self.ui.squareWave.isChecked():
                pinData['waveForm'] = 'square'
            self.ui.sineWave.setEnabled(True)
            self.ui.squareWave.setEnabled(True)
            self.ui.maxAnalogValue.setEnabled(True)
            self.ui.serialLineEdit.setEnabled(False)
        elif self.ui.digital.isChecked():
            pinData['dataType'] = 'digital'
            self.ui.sineWave.setEnabled(False)
            self.ui.squareWave.setEnabled(False)
            self.ui.maxAnalogValue.setEnabled(False)
            self.ui.serialLineEdit.setEnabled(False)
        elif self.ui.serial.isChecked():
            self.ui.sineWave.setEnabled(False)
            self.ui.squareWave.setEnabled(False)
            self.ui.maxAnalogValue.setEnabled(False)
            self.ui.serialLineEdit.setEnabled(True)
            pinData['dataType'] = 'serial'
            pinData['serialLineEdit'] = self.ui.serialLineEdit.text()
        else:
            raise ConfigurationError("Unknown pin type.")

        self.settings[self.getCurrentSignalId()] = pinData

    def resetData(self):
        "Resets the currently stored data."
        self.settings = {}

    def browse(self):
        "Browers for listing emulation files."
        (filename, _) = QtGui.QFileDialog.getOpenFileName(self, "Find Files",
                QtCore.QDir.currentPath())

        if filename:
            if not self.ui.studentFiles.findItems(filename, QtCore.Qt.MatchExactly):
                self.ui.studentFiles.addItem(filename)

            index = self.ui.studentFiles\
                               .indexFromItem(self.ui.studentFiles\
                                    .findItems(filename, QtCore.Qt.MatchExactly)[0])
            self.ui.studentFiles.setCurrentIndex(index)


    def openScenario(self):
        """
        Open a given scenario's configuration.
        """
        fileName, _ = QtGui.QFileDialog.getOpenFileName(self, "Open File",
            QtCore.QDir.currentPath())
        if fileName:
            with open(fileName, 'r') as f:
                self.settings = pickle.load(f)
                self.ui.outputPinsList.clear()
                for x in self.settings:
                    self.ui.outputPinsList.addItem(x)

    def saveScenario(self):
        "Saves the settings"
        print("Saveing", self.settings)
        print("Pickled", pickle.dumps(self.settings))
        (filename, _) = QtGui.QFileDialog.getSaveFileName(self)
        print('filename:', filename)
        if not filename:
            return
        if not filename.endswith('.cfg'):
            filename += '.cfg'

        print(filename)
        with open(filename, 'w+') as f:
            self._saveAs(f)

    def _saveAs(self, filename):
        "Save the settings to a specified file."
        pickle.dump(self.settings, filename)

    @QtCore.Slot(int)
    def onOutputPinsListSelect(self, value, previous_value):
        "Update the form to match the saved data."
        self.ui.exponential.setChecked(False)
        self.ui.expLambda.clear()
        self.ui.expLambda.setEnabled(False)
        self.ui.expDuration.clear()
        self.ui.expDuration.setEnabled(False)
        self.ui.uniA.clear()
        self.ui.uniA.setEnabled(False)
        self.ui.uniB.clear()
        self.ui.uniB.setEnabled(False)
        self.ui.uniDuration.clear()
        self.ui.uniDuration.setEnabled(False)

        self.ui.digital.setChecked(False)
        self.ui.maxAnalogValue.clear()
        self.ui.serialLineEdit.clear()

        self.ui.exponential.setEnabled(True)
        self.ui.uniform.setEnabled(True)
        self.ui.deterministic.setEnabled(True)
        
        self.ui.digital.setEnabled(True)
        self.ui.analog.setEnabled(True)
        self.ui.serial.setEnabled(True)

        self.ui.sineWave.setEnabled(False)
        self.ui.squareWave.setEnabled(False)
        self.ui.maxAnalogValue.setEnabled(False)
        self.ui.serialLineEdit.setEnabled(False)

        if value.text() in self.settings:
            pinData = self.settings[value.text()]
            if pinData['signalType'] == 'exp':
                self.ui.exponential.setChecked(True)
                self.ui.expLambda.setEnabled(True)
                self.ui.expLambda.setText(pinData['expLambda'])
                self.ui.expDuration.setEnabled(True)
                self.ui.expDuration.setText(pinData['expDuration'])
            elif pinData['signalType'] == 'uni':
                self.ui.uniform.setChecked(True)
                self.ui.uniA.setEnabled(True)
                self.ui.uniA.setText(pinData['uniA'])
                self.ui.uniB.setEnabled(True)
                self.ui.uniB.setText(pinData['uniB'])
                self.ui.uniDuration.setEnabled(True)
                self.ui.uniDuration.setText(pinData['uniDuration'])
            else:
                self.ui.expLambda.setEnabled(True)
                self.ui.expDuration.setEnabled(True)

            if pinData['dataType'] == 'digital':
                self.ui.digital.setChecked(True)
            elif pinData['dataType'] == 'analog':
                self.ui.analog.setChecked(True)
                self.ui.sineWave.setEnabled(True)
                self.ui.squareWave.setEnabled(True)
                if pinData['waveForm'] == 'square':
                    self.ui.squareWave.setChecked(True)
                else:
                    self.ui.sineWave.setChecked(True)
                self.ui.maxAnalogValue.setEnabled(True)
                self.ui.maxAnalogValue.setText(pinData['maxAnalogValue'])
            elif pinData['dataType'] == 'serial':
                self.ui.serial.setChecked(True)
                self.ui.serialLineEdit.setEnabled(True)
                self.ui.serialLineEdit.setText(pinData['serialLineEdit'])
            else:
                self.ui.digital.setChecked(True)
        else:
            self.ui.expLambda.setEnabled(True)
            self.ui.expDuration.setEnabled(True)

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ui_configuration = CWindow()
    main_window = MainWindow(ui_configuration)
    main_window.show()
    sys.exit(app.exec_())

