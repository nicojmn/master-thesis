---
marp: true
theme: default
title: "Master's thesis meeting : week 4"
footer: "Direct device access from the SmartNIC towards datacenter disagregation (Nicolas Jeanmenne)"
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

# Direct device access from the SmartNIC towards datacenter disagregation

## Master's thesis meeting : Week 4

### Nicolas Jeanmenne

---
## Table of contents




- [SmartNIC / NVMe-oF](#smartnic--nvme-of)
- [Paper writing](#paper-writing)
- [Conclusion](#conclusion)
- [TODOs for week 5](#todos-for-week-5)

<!-- <div class="container">

<div class="col">

</div>

<div class="col">

</div>
</div> -->

---

# SmartNIC / NVMe-oF

- Use the SmartNIC as the initiator for `basic-cp`and the KV store
  
- Idea : SNIC cores for control and no host CPU utilization

- How to "expose" traditional linux FS to the SNIC ?
  - Hypothesis : DPU-designed FS
  - Approach : rely on literature and IO-TCP code
  - Problem : IO-TCP code isn't available anymore

---

# Paper writing

- Currently draft notes from SOTA
- Inspiration for shape / outline from Nikita, Maxime, ... thesis on DIAL
- <https://github.com/nicojmn/DDA-SNIC-disaggregation/blob/writing/writing/main.pdf> (link will be provided on Teams after the meeting)

---


# Conclusion

- Investigated on NVMe-oF
- Template setup for paper

---

# TODOs for week 5

- Integrate previous programs and KV store with NVMe-oF
- Measurements
- Paper writing (restructuring, table comparisons, ...)

---

# That's all for today !
