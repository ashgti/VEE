import sys
from math import pi, degrees
from PySide import QtCore, QtGui

class RenderArea(QtGui.QWidget):
    """Render area for rendering the robot"""
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        
        self.current_time = 0.0
        self.lines = []
        
        newFont = self.font()
        newFont.setPixelSize(12)
        self.setFont(newFont)
        
        self.robot_rotation = 180
        self.robot_position = (0, 0)
        
        fontMetrics = QtGui.QFontMetrics(newFont)
        self.xBoundingRect = fontMetrics.boundingRect(self.tr("x"))
        self.yBoundingRect = fontMetrics.boundingRect(self.tr("y"))
    
    def paintEvent(self, event):
        """Handles painting the render area"""
        painter = QtGui.QPainter()
        painter.begin(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)
        painter.fillRect(event.rect(), QtGui.QBrush(QtCore.Qt.black))
        
        painter.save()
        self.drawGlobalCoordinates(painter)
        painter.restore()
        
        painter.save()
        self.drawTime(painter)
        painter.restore()
        
        # Correct origin location and rotation
        painter.translate(self.size().width()/2, self.size().height()/2)
        painter.rotate(180)
        
        self.drawReticle(painter)
        
        self.drawRobotPath(painter)
        
        painter.save()
        self.drawRobot(painter)
        painter.restore()
        
        painter.end()
    
    def updateDisplay(self, x, y, w, time):
        """docstring for updateDisplay"""
        self.current_time = time
        x_, y_ = self.robot_position
        self.robot_position = (x*100.0, y*100.0)
        self.robot_rotation = degrees(w)
        
        if x_ != x*100.0 and y_ != y*100.0:
            self.lines.append(QtCore.QLineF(x_, y_, x*100.0, y*100.0))
        
        self.update()
    
    def reset(self):
        """Resets the render area"""
        self.current_time = 0.0
        self.lines = []
        self.robot_rotation = 180.0
        self.robot_position = (0.0, 0.0)
        self.update()
    
    def drawRobotPath(self, painter):
        """Draws the path of the robot"""
        for line in self.lines:
            painter.drawLine(line)
    
    def drawTime(self, painter):
        """Draw the time"""
        painter.setPen(QtCore.Qt.white)
        painter.drawText(self.size().width() - 50, self.size().height() - 10, str(self.current_time))
    
    def drawReticle(self, painter):
        """Draws a little reticle at the origin"""
        painter.setPen(QtCore.Qt.gray)
        painter.drawLine(-10, 0, 10, 0)
        painter.drawLine(0, -10, 0, 10)
    
    def drawRobot(self, painter):
        """Draws the robot on the screen"""
        painter.translate(*self.robot_position)
        painter.rotate(self.robot_rotation)
        painter.setPen(QtCore.Qt.yellow)
        painter.drawEllipse(-50, -50, 100, 100)
        painter.drawRect(-15, -40, 30, 10)
        painter.drawRect(-15, 30, 30, 10)
        self.drawCoordinates(painter)
    
    def drawCoordinates(self, painter):
        painter.setPen(QtCore.Qt.red)
        
        # painter.rotate(180)
        
        painter.drawLine(0, 0, 50, 0)
        painter.drawLine(48, -2, 50, 0)
        painter.drawLine(48, 2, 50, 0)
        painter.save()
        painter.translate(48, -12)
        text_doc = QtGui.QTextDocument()
        text_doc.setHtml('<p style="color: red; font: 12px;">X<sub>R</sub></p>')
        text_doc.drawContents(painter)
        painter.restore()
        
        painter.drawLine(0, 0, 0, -50)
        painter.drawLine(-2, -48, 0, -50)
        painter.drawLine(2, -48, 0, -50)
        painter.save()
        painter.translate(-8, -70)
        text_doc = QtGui.QTextDocument()
        text_doc.setHtml('<p style="color: red; font: 12px;">Y<sub>R</sub></p>')
        text_doc.drawContents(painter)
        painter.restore()
        
    
    def drawGlobalCoordinates(self, painter):
        offset_x = 10
        offset_y = self.size().height() - 10
        painter.translate(offset_x, offset_y)
        
        painter.setPen(QtCore.Qt.red)
        
        painter.drawLine(0, 0, 50, 0)
        painter.drawLine(48, -2, 50, 0)
        painter.drawLine(48, 2, 50, 0)
        painter.save()
        painter.translate(48, -12)
        text_doc = QtGui.QTextDocument()
        text_doc.setHtml('<p style="color: red; font: 12px;">X<sub>I</sub></p>')
        text_doc.drawContents(painter)
        painter.restore()
        
        painter.setPen(QtCore.Qt.green)
        
        painter.drawLine(0, 0, 0, -50)
        painter.drawLine(-2, -48, 0, -50)
        painter.drawLine(2, -48, 0, -50)
        painter.save()
        painter.translate(-8, -70)
        text_doc = QtGui.QTextDocument()
        text_doc.setHtml('<p style="color: rgb(0,247,0); font: 12px;">Y<sub>I</sub></p>')
        text_doc.drawContents(painter)
        painter.restore()
    
