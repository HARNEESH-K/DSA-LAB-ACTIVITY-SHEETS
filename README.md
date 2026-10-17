# DSA Capstone Project: Browser History Management Using Data Structures

**Course:** 23CSE203 - Data Structures and Algorithms  
**Academic Institution:** Amrita School of Computing, Amrita Vishwa Vidyapeetham, Chennai Campus  
**Review Stage:** Phase 2 & 3 Review  

### 👥 Team Members
- **Harneesh K** &bull; CH.SC.U4CSE25217 &bull; CSE Section C
- **Harish G** &bull; CH.SC.U4CSE25259 &bull; CSE Section C

### 👨‍🏫 Faculty Mentors
- **Dr. S. Rajiv** (Department of Computer Science and Engineering)
- **G. Bharathi Mohan** (Course Mentor)

---

## 🌟 Project Overview

This capstone project delivers a high-performance **Browser History Management Engine** implemented using a **Doubly Linked List (DLL)**. It simulates the exact navigation behavior of modern web browsers (Google Chrome, Firefox, Safari) with $O(1)$ constant-time forward/backward navigation and dynamic branch pruning with zero memory leaks.

### 🚀 Key Features

1. **Full Interactive Web Browser Simulator:**
   - Multi-Tab Navigation: Switch between Tab 1, Tab 2, and new tabs, each running an isolated Doubly Linked List instance.
   - Omnibox Address Bar: Direct URL navigation with HTTPS security badge and bookmarks bar.
   - Pre-loaded Demo Scenarios: Replicate the presentation flow (`Google → YouTube → Instagram → ChatGPT → Back x2 → Wikipedia`) with one click.
   - Realistic Webpage Viewports: Contextual simulated rendering of visited sites with HTTP 200 responses.

2. **Real-Time Doubly Linked List Visualizer:**
   - Visualizes every node in memory with simulated 32-bit addresses (`0x...`).
   - Displays `prev` and `next` pointer linkages.
   - Glowing indicator pointing to the `CURRENT` active webpage node.
   - Direct Jump: Click any node to instantly traverse to that point in history.
   - Visual Memory Pruning: Dropped forward nodes flash and detach cleanly when a new page is visited after navigating backward.

3. **Data Structure Architectural Comparison:**
   - Head-to-head comparison of **Doubly Linked List** vs **Single Stack** vs **Two-Stack Model** vs **Dynamic Array**.
   - Asymptotic complexity breakdown ($O(1)$ vs $O(N)$).
   - Real-time Canvas performance graph illustrating why DLL maintains flat latency regardless of scale.

4. **Automated 10/10 Test Suite (Slide 6 Verification):**
   - Directly implements all 10 tests from Slide 6 of the Capstone Presentation:
     - **T1:** Visit 4 pages &rarr; Verify current is ChatGPT
     - **T2-T3:** Back twice &rarr; Instagram, then YouTube
     - **T4:** Forward once &rarr; Instagram
     - **T5:** Back at first page &rarr; Gracefully blocked (NULL pointer boundary check)
     - **T6:** Forward at last page &rarr; Gracefully blocked (NULL pointer boundary check)
     - **T7-T8:** Visit after Back &rarr; Forward branch dropped, order preserved
     - **T9:** Search "wiki" &rarr; Located in $O(N)$ time
     - **T10:** 100,000 pages scale test &rarr; Verified in ~0.26s
   - "Run Automated Test Suite" button with live timers and status pills.

5. **Slide-by-Slide Presentation Viewer (17 Slides):**
   - Embedded interactive presentation viewer covering all 17 presentation slides with speaker notes.

6. **Faculty Viva Voce Guide:**
   - Comprehensive Q&A covering pointer manipulation, heap deallocation in C, and why circular lists are unsuitable for browser history.

---

## 💻 How to Run the Website

### Option 1: Direct File Launch (No dependencies needed)
Double-click `index.html` in file explorer or open it with Google Chrome, Microsoft Edge, or Firefox:
```bash
# In Windows PowerShell:
Start-Process "index.html"
```

### Option 2: Run C Code Directly
The native C implementation is provided in `browser_history.c`:
```bash
gcc -Wall -Wextra -std=c99 browser_history.c -o browser_history.exe
./browser_history.exe
```
