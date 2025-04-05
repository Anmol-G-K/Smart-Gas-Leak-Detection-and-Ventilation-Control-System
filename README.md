<h1 align="center">Smart Gas Leak Detection and Ventilation Control System</h1>

<p align="center">
  <img src="assets/icons/microcontroller.png" width="200">
</p>

<details>
  <summary><h2>📚 Table of Contents (Click to Expand)</h2></summary>

- [📌 Project Overview](#-project-overview)
- [👥 Project Members](#-project-members)
- [🚀 Features](#-features)
- [⚙️ How It Works](#️-how-it-works)
  - [🔹 **Gas Leak Detection**](#-gas-leak-detection)
  - [🔹 **LCD Display \& Alerts**](#-lcd-display--alerts)
- [🛠️ Components Used](#️-components-used)
  - [**🔍 Sensors \& Inputs**](#-sensors--inputs)
  - [**⚡ Outputs \& Actuators**](#-outputs--actuators)
- [🖥️ Installation](#️-installation)
- [🔌 Hardware Connections for Gas Leak Detection System](#-hardware-connections-for-gas-leak-detection-system)
- [📈 System Flowchart](#-system-flowchart)
- [📜 License](#-license)
- [📩 Contact](#-contact)

</details>

## 📌 Project Overview  
The **Smart Gas Leak Detection and Ventilation Control System** enhances safety in homes, kitchens, and industrial spaces by detecting gas leaks and triggering an exhaust fan to **prevent hazardous gas accumulation**.  

It utilizes an **MQ-6 gas sensor** to detect leaks and automatically activates an **exhaust fan** when gas levels exceed a threshold, ensuring a **safe environment**.  

---

## 👥 Project Members  
- **Anmol Govindarajapuram Krishnan** – [Linkedin](https://www.linkedin.com/in/anmolkrish/)
- **Karthik K** – [Linkedin]()
---
## 🚀 Features  
- **Automatic gas leak detection**  
- **Real-time LCD monitoring**  
- **Buzzer alert system**  
- **Exhaust fan activation for safety**  

---

## ⚙️ How It Works  

### 🔹 **Gas Leak Detection**  
- The **MQ-6 gas sensor** continuously monitors the surroundings for **Methane, Butane, LPG, and smoke**.  
- If gas levels exceed a safety threshold, the **exhaust fan is triggered** to remove the gas.  

### 🔹 **LCD Display & Alerts**  
- **Real-time display** of gas concentration.  
- **Buzzer alarm** alerts users when gas leaks are detected.  

---

## 🛠️ Components Used  

### **🔍 Sensors & Inputs**  
- **MQ-6 Gas Sensor** – Detects hazardous gases like **Methane, Butane, LPG, and smoke**.  

### **⚡ Outputs & Actuators**  
- **PWM-controlled Exhaust Fan** – Automatically turns on when gas levels exceed the threshold.  
- **LCD Display** – Shows gas concentration levels.  
- **Buzzer** – Sounds an alert in case of a gas leak.  
- **Motor Driver (MOSFET-based)** – Controls fan motor speed.  

---

## 🖥️ Installation  
- The system is programmed using **MPLAB IDE**,  visit the [MPLAB IDE Download Page](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem). *```(ver >= 5.10)```*.  
- Install the XC8 Compiler [XC8 Compiler Download Page](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers#tabs) *```(ver >= 2.50)```*.

---

## 🔌 Hardware Connections for Gas Leak Detection System

![Circuit Diagram](\designs\ckt.bmp)

---

## 📈 System Flowchart

![System Flowchart](\designs\flowchart.png)

---

## 📜 License  
- This project is licensed under the **MIT License**. Refer to the [LICENSE](LICENSE) file for more details.  

---

## 📩 Contact  
For inquiries or collaboration, reach out to the project members.  

---