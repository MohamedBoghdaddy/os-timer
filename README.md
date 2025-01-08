# ⏰ **PintOS Alarm Clock**  

A project to enhance the PintOS operating system by implementing an efficient alarm clock mechanism. The primary goal is to replace the existing busy-waiting logic with a sleep queue, improving CPU utilization and overall system performance.

---

## 🌟 **Features**
- Implements a sleep queue for thread management.
- Efficient `timer_sleep` function using a `BLOCKED` state.
- Wakes up threads based on tick values without consuming CPU cycles.
- Modular and scalable design for future enhancements.

---

## 🛠 **Tech Stack**
- **Language**: C  
- **Framework**: PintOS  
- **Platform**: QEMU (x86 architecture)  

---

## 🗂 **Project Structure**
### Key Files:
- `src/threads/thread.h`  
  - Added the `wakeup_tick` field to the `struct thread`.  
  - Defined function prototypes for sleep/wakeup mechanisms.  

- `src/threads/thread.c`  
  - Implemented the sleep queue (`sleep_list`) and associated functions like `thread_sleep`, `thread_wake`.  

- `src/devices/timer.h`  
  - Added declarations for new timer functions.  

- `src/devices/timer.c`  
  - Modified `timer_sleep` to avoid busy waiting.  
  - Updated `timer_interrupt` to check and wake sleeping threads.  

---

## ⚙️ **Installation & Setup**
1. Clone the PintOS repository:
   ```bash
   git clone https://github.com/your-repo/pintos.git
   cd pintos
   ```

2. Navigate to the `src/threads` directory:
   ```bash
   cd src/threads
   ```

3. Build the project:
   ```bash
   make
   ```

4. Test the implementation:
   ```bash
   pintos --qemu -- -q run alarm-multiple
   ```

---

## 🚀 **How It Works**
### **Overview**
The alarm clock works by:
1. Storing sleeping threads in a **sleep queue** (`sleep_list`).
2. Using a **wakeup_tick** to specify when a thread should wake up.
3. Leveraging the `BLOCKED` state to avoid busy waiting.

### **Execution Flow**
- When a thread calls `timer_sleep`, it is moved to the sleep queue and marked as `BLOCKED`.
- The `timer_interrupt` checks the sleep queue every tick.
- Threads whose `wakeup_tick` has elapsed are moved back to the ready queue.

---

## 📝 **Project Requirements**
1. Modify the following files:
   - `thread.h`
   - `thread.c`
   - `timer.h`
   - `timer.c`
2. Implement:
   - Sleep queue (`sleep_list`).
   - Thread management logic (`thread_sleep`, `thread_wake`).
   - Efficient `timer_sleep` without busy waiting.

3. Validate functionality with test cases:
   - Ensure CPU utilization improves.
   - Compare idle ticks before and after implementation.

---

## 📊 **Results**
- **Before:** High CPU usage due to busy waiting in `timer_sleep`.  
- **After:** CPU idle time increases as threads use the `BLOCKED` state efficiently.  

---

## 🏆 **Grading Criteria**
- **Implementation of requirements:** 10 marks  
- **Oral discussion and demonstration:** 5 marks  

---

## 📌 **References**
- [PintOS Documentation](https://web.stanford.edu/class/cs140/projects/pintos/pintos_1.html)  
- Supporting lecture slides and videos (refer to course material).  

---

## 📄 **License**
This project follows the PintOS license.
