"""
MainWindow is the driver program for the GUI components of VEE.
"""

import sys, os
import pickle

try:
    from PySide import QtCore, QtGui
except ImportError as e:
    sys.path.insert(0, os.path.join(os.getcwd(), 'lib', 'python2.7','lib-dynload')) ## Added to fix dynlib bug
    from PySide import QtCore, QtGui
import vee.ardulator as a
from vee.configuration_ui import Ui_MainWindow as CWindow


class ApplicationRunner(object):
    def __init__(self, src, dest):
        self.output = None

    def make(self):
        """
        Operations:
          1. Copy Files to a new location.
          2. Extract Files if compressed.
          3. Compile against students code.
        """
        pass

    def run(self):
        """
        Run the scenario with the current configuration files.
        """
        pass

    def results(self):
        """
        Return the results of the run.
        """
        pass

class ConfigurationError(ValueError):
    pass

class MainWindow(QtGui.QMainWindow):
    "Main application window."
    list_changed = QtCore.Signal()

    def __init__(self, ui):
        "Initialize the Window, connects the UI components and initializes variables."
        super(MainWindow, self).__init__()
        # QtGui.QMainWindow.__init__(self)
        self.ui = ui
        self.settings = {}
        self.pinTypeReference = [["Disabled", self.hideAllForms],
                                 ["Exponential Digital", self.showExponential],
                                 ["Exponential Analog", self.showExponential],
                                 ["Exponential Serial Output", self.showExponential],
                                 ["Uniform Digital", self.showUniform],
                                 ["Uniform Analog", self.showUniform],
                                 ["Uniform Serial Output", self.showUniform]]

        self.ui.setupUi(self)
        self.connectComponents()
        self.ui.outputPinsList.setCurrentRow(0)

    def saveConfiguration(self):
        print self.settings

    def connectComponents(self):
        "Connects the various UI components to python functions."
        ui = self.ui
        ui.outputPinsList.currentRowChanged\
                .connect(self.onOutputPinsListSelect)
        # ui.pinType.currentIndexChanged.connect(self.onPinSelection)

        # Menu Actions
        ui.actionNew_Scenario.triggered.connect(self.resetData)
        ui.actionSave.triggered.connect(self.saveScenario)

        # Add Validators
        ui.uni_A.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uni_A))
        ui.uni_A.textEdited.connect(self.updatePinConfiguration)
        ui.uni_B.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uni_B))
        ui.uni_B.textEdited.connect(self.updatePinConfiguration)
        ui.uni_Duration.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.uni_Duration))
        ui.uni_Duration.textEdited.connect(self.updatePinConfiguration)

        ui.exp_Lambda.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.exp_Lambda))
        ui.exp_Lambda.textEdited.connect(self.updatePinConfiguration)
        ui.exp_Duration.setValidator(QtGui.QDoubleValidator(0.0, float("inf"), 10, ui.exp_Duration))
        ui.exp_Duration.textEdited.connect(self.updatePinConfiguration)

        # Button Behaviors
        ui.browserFiles.clicked.connect(self.browse)

        self.setupPinType()
        self.hideAllForms()

    def getCurrentPinId(self):
        """docstring for getCurrentPinId"""
        return self.ui.outputPinsList.currentRow()

    def updatePinConfiguration(self):
        pass
    #     num_index = self.ui.pinType.currentIndex()
    #     pinData = { 'type' : num_index }
    #     if 0 < num_index < 4:
    #         pinData['exp_Lambda'] = self.ui.exp_Lambda.text()
    #         pinData['exp_Duration'] = self.ui.exp_Duration.text()
    #     elif 3 < num_index < 10:
    #         pinData['uni_A'] = self.ui.uni_A.text()
    #         pinData['uni_B'] = self.ui.uni_B.text()
    #         pinData['uni_Duration'] = self.ui.uni_Duration.text()
    #     else:
    #         raise ConfigurationError("Unknown pin type.")
    #     self.settings[self.getCurrentPinId()] = pinData
    #     print self.settings
    #     print pinData

    def resetData(self):
        self.settings = {}
        
    def browse(self):
        (filename, _) = QtGui.QFileDialog.getOpenFileName(self, "Find Files",
                QtCore.QDir.currentPath())

        print filename

        if filename:
            if not self.ui.studentFiles.findItems(filename, QtCore.Qt.MatchExactly):
                self.ui.studentFiles.addItem(filename)

            index = self.ui.studentFiles\
                               .indexFromItem(self.ui.studentFiles\
                                    .findItems(filename, QtCore.Qt.MatchExactly)[0])
            self.ui.studentFiles.setCurrentIndex(index)


    def saveScenario(self):
        print 'Help'
        print "Saveing", self.settings
        print "Pickled", pickle.dumps(self.settings)
        (filename, _) = QtGui.QFileDialog.getSaveFileName(self)
        print 'filename:', filename
        if not filename.endswith('.cfg'):
            filename += '.cfg'
        
        print filename
        with open(filename, 'w+') as f:
            pickle.dump(self.settings, f)

    def runScenario(self):
        self.ui.hide()

    def saveFile(self, fileName):
        file = QtCore.QFile(fileName)
        if not file.open(QtCore.QFile.WriteOnly | QtCore.QFile.Text):
            QtGui.QMessageBox.warning(self, "Application",
                    "Cannot write file %s:\n%s." % (fileName, file.errorString()))
            return False

        outf = QtCore.QTextStream(file)
        QtGui.QApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        outf << self.textEdit.toPlainText()
        QtGui.QApplication.restoreOverrideCursor()

        self.setCurrentFile(fileName);
        self.statusBar().showMessage("File saved", 2000)
        return True

    def setupPinType(self):
        """Configure Pin Types"""
        # for index, name in enumerate(self.pinTypeReference):
        #     self.ui.pinType.insertItem(index,
        #             QtGui.QApplication\
        #                     .translate("MainWindow",
        #                                name[0],
        #                                None,
        #                                QtGui.QApplication.UnicodeUTF8))

    @QtCore.Slot(int)
    def onOutputPinsListSelect(self, value):
        "Update the form to match the saved data."
        if value in self.settings:
            num_index = self.settings[value]['type']
            self.ui.pinType.setCurrentIndex(self.settings[value]['type'])
            if 0 < num_index < 4:
                self.ui.exp_Lambda.setText(self.settings[value]['exp_Lambda'])
                self.ui.exp_Duration.setText(self.settings[value]['exp_Duration'])
            elif 3 < num_index < 10:
                self.ui.uni_A.setText(self.settings[value]['uni_A'])
                self.ui.uni_B.setText(self.settings[value]['uni_B'])
                self.ui.uni_Duration.setText(self.settings[value]['uni_Duration'])
            else:
                raise ConfigurationError("Unknown pin type.")
            self.hideAllForms()
            self.pinTypeReference[num_index][1]()
        else:
            self.ui.pinType.setCurrentIndex(0)
            self.ui.exp_Lambda.clear()
            self.ui.exp_Duration.clear()
            self.ui.uni_A.clear()
            self.ui.uni_B.clear()
            self.ui.uni_Duration.clear()

    @QtCore.Slot(str)
    def onPinSelection(self, index):
        if index > 0:
            self.ui.exp_Lambda.clear()
            self.ui.exp_Duration.clear()
            self.ui.uni_A.clear()
            self.ui.uni_B.clear()
            self.ui.uni_Duration.clear()
        self.pinTypeReference[index][1]()

    def hideAllForms(self):
        "Hides all the editable forms."
        # self.ui.exponential.disable()
        # self.ui.uniform.disable()
        pass

    def showExponential(self):
        "Shows the Exponential Configuration Settings."
        self.hideAllForms()
        # self.ui.exponential.enable()

    def showUniform(self):
        "Show the Uniform Configuration Settings"
        self.hideAllForms()
        # self.ui.uniform.enable()

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ui_configuration = CWindow()
    main_window = MainWindow(ui_configuration)
    main_window.show()
    sys.exit(app.exec_())
