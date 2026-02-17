---
marp: true
theme: default
title: "Master's thesis meeting : week 9"
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

## Master's thesis meeting : week 9

### Nicolas Jeanmenne

---
## Table of contents


<!-- <div class="container">

<div class="col">

</div>

<div class="col">

</div>
</div> -->

- [SOTA updates](#sota-updates)
- [SPDK progress](#spdk-progress)
- [Objectives](#objectives)
- [Poster](#poster)
- [Conclusion](#conclusion)
- [TODOs for week 11](#todos-for-week-11)


---

# SOTA updates

- Found additional papers for storage offloading / handling
  
---

![bg contain](img/S9/newpapers.png)

---

# SPDK progress

- Finished setup on Frodo
- Tried examples scripts like *bdevperf*

---

# Objectives

- Read for inspiration on papers
- Split data like TCP/IO but for NVMe (still a fuzzy idea)
  - Seems like what could be gain in flexibility would be lost in performance and vice versa
- **I'm lost**

---

# Poster

- Context : speak about resource stranding
- Problem : not found yet
- Approach :
  - data transfer with SPDK on local server
  - transfer on the network with DPDK
  - If enough time, encryption

---


# Conclusion

- Working on paper creation
- Added a few papers to know more about storage offlading


---

# TODOs for week 11

- More SPDK scripts
- I hope to find what the thesis problem is by week 11
- Investigate NVMe Computational SSD (it is interesting for our storage disaggregation ?)

---

# That's all for today !
