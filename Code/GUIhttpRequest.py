from time import localtime, strftime
from Tkinter import *
import httplib

class Application(Frame):
        """A GUI Application for BlindZer"""

        def __init__(self, master):
                """Initialize the Frame"""
                Frame.__init__(self,master)
                self.grid()
                self.IP = "192.168.0.125"
                self.create_widgets()

        def create_widgets(self):
                """Create Widgets."""
                #create label
                self.label = Label(self)
                #self.label["text"] = "Current Time: " + strftime("%Y-%m-%d %H:%M:%S",  localtime())
                self.label["text"] = "Please choose the operation"
                self.label.grid(row=0, column=0, columnspan=2, sticky=N)
                #create button
                self.button_1 = Button(self)
                self.button_1["text"] = "Roll Up"
                self.button_1["command"] = self.send_http_request_1
                self.button_1.grid(row=1, column=0, columnspan=1, sticky=W)
                #create button
                self.button_2 = Button(self)
                self.button_2["text"] = "Roll Down"
                self.button_2["command"] = self.send_http_request_2
                self.button_2.grid(row=1, column=1, columnspan=1, sticky=E)
        def send_http_request_1(self):
                conn = httplib.HTTPConnection(self.IP)
                conn.request("PUT","/Servo=UP")
        def send_http_request_2(self):
                conn = httplib.HTTPConnection(self.IP)
                conn.request("PUT","/Servo=DOWN")

root = Tk()
root.title("BlindZer")
#root.geometry("208x55")
root.geometry("171x53")

app = Application(root)
root.mainloop()

