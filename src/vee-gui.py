"""
MainWindow is the driver program for the GUI components of VEE.
"""

import sys, os

try:
    from PySide import QtCore, QtGui
except ImportError as e:
    sys.path.insert(0, os.path.join(os.getcwd(), 'lib', 'python2.7','lib-dynload')) ## Added to fix dynlib bug
    from PySide import QtCore, QtGui
import vee
from vee.configuration_ui import Ui_MainWindow as CWindow
from vee.results_ui import Ui_MainWindow as RWindow

class ResultsWindow(QtGui.QMainWindow):
    "A testing window, will run the simulation"
    def __init__(self, ui):
        QtGui.QMainWindow.__init__(self)
        self.ui = ui
        self.ui.setupUi(self)

    def connectUi(self):
        pass

class MainWindow(QtGui.QMainWindow):
    "Main application window."
    list_changed = QtCore.Signal()

    def __init__(self, ui, scenario_runner):
        "Initialize the Window, connects the UI components and initializes variables."
        QtGui.QMainWindow.__init__(self)
        self.ui = ui
        self.settings = {}
        self.pinData = None

        self.scenario_runner = scenario_runner
        self.scenario_runner.show()

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
        ui.pinType.currentIndexChanged.connect(self.onPinSelection)
        self.ui.actionOpen_Scenario_Runner.triggered.connect(self.runScenario)
        self.setupPinType()
        self.hideAllForms()

    def runScenario(self):
        print 'yup'
        self.ui.hide()

    def setupPinType(self):
        ""
        for index, name in enumerate(self.pinTypeReference):
            self.ui.pinType.insertItem(index,
                    QtGui.QApplication\
                            .translate("MainWindow",
                                       name[0],
                                       None,
                                       QtGui.QApplication.UnicodeUTF8))

    @QtCore.Slot(int)
    def onOutputPinsListSelect(self, value):
        ""
        self.updatePinConfiguraiton(value)

    @QtCore.Slot(str)
    def onPinSelection(self, index):
        ""
        if self.pinData:
            num_index = self.ui.pinType.currentIndex()
            self.pinData['type'] = num_index
            self.settings[self.pinData['id']] = self.pinData
            self.pinTypeReference[num_index][1]()

    def hideAllForms(self):
        "Hides all the editable forms."
        self.ui.exponential.hide()
        self.ui.uniform.hide()
        # print self.pinData
        # if self.pindData and self.pinData['id']:
        #     self.pinData = None
        #     self

    def showExponential(self):
        "Shows the Exponential Configuration Settings."
        self.hideAllForms()
        self.ui.exponential.show()

    def showUniform(self):
        "Show the Uniform Configuration Settings"
        self.hideAllForms()
        self.ui.uniform.show()

    def updatePinConfiguraiton(self, pin):
        ""
        try:
            data = self.settings[pin]
        except KeyError:
            data = {'id': pin, 'type': 0}
            self.pinData = data
            self.settings[pin] = data

        try:
            self.ui.pinConfigurationLabel.setText("Pin ID: " + str(data['id']))
            self.ui.pinType.setCurrentIndex(data['type'])
        except Exception as e:
            print e

if __name__ == '__main__':
    print 'main'
    app = QtGui.QApplication(sys.argv)
    ui_configuration = CWindow()
    ui_scenario = RWindow()
    main_window = MainWindow(ui_configuration, ResultsWindow(ui_scenario))
    main_window.show()
    sys.exit(app.exec_())
