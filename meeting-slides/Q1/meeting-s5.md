---
marp: true
theme: default
title: "Master's thesis meeting : week 5"
footer: "Direct device access from the SmartNIC towards datacenter disaggregation (Nicolas Jeanmenne)"
paginate: true
math: katex
---

<style>
.container{
    display: flex;
}
.col{
    flex: 1;
}
</style>

# Direct device access from the SmartNIC towards datacenter disaggregation

## Master's thesis meeting : week 5

### Nicolas Jeanmenne

---
## Table of contents

1. [NVMe-oF](#nvme-of)
   1. NVMe-over-Fabrics Performance Characterization and the Path to Low-Overhead Flash Disaggregation
   2. Lightpool
   3. Scalio and NVMe target offloading
2. [Conclusion](#conclusion)
3. [TODOs for week 7](#todos-for-week-7)

<!-- <div class="container">

<div class="col">

</div>

<div class="col">

</div>
</div> -->

---

# NVMe-oF

- [NVMe-oF Performance Characterization and the Path to Low-Overhead Flash Disaggregation](https://dl.acm.org/doi/pdf/10.1145/3078468.3078483)
  - Analyze NVMe-oF / SPDK and validates it's usage with a smartNIC
  - $\approx 12 \mu s$ overhead compared to direct-attached software $\Rightarrow$ implementation on *Frodo* should have similar overhead 
- [Lightpool](https://ieeexplore.ieee.org/abstract/document/10476433) : made it possible to use NVMe-oF to transfer data into distributed DB.
  - SmartNIC could be combined with NVMe-oF to offload CPU
- [Scalio](https://www.usenix.org/system/files/osdi25-sun.pdf) : uses NVMe-oF to offload ARM cores on the smartNIC
  - NVMe-oF target offload : standard implementation on the server-side DPU's host channel adapter via PCIe P2P

---

# Conclusion

- Lightpool and Scalio show that using NVME-oF with a smartNIC appears to be a viable approach
- NVMe-oF target offloading looks even more promising to reduce the smartNIC ARM cores workload via PCIe P2P
  - ARM cores should only be used for the control path
  - Care must be taken about data coherence between smartNIC and SSDs

---

# TODOs for week 7

- Finish SPDK setup on *Frodo*
- Investigate SPDK to write data blocks from the smartNIC towards NVMe

---

# That's all for today !
