---
marp: true
theme: default
title: "Master's thesis meeting : week 11"
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

## Master's thesis meeting : week 11

### Nicolas Jeanmenne

---
## Table of contents


<!-- <div class="container">

<div class="col">

</div>

<div class="col">

</div>
</div> -->

- [Poster](#poster)
- [About computational SSD](#about-computational-ssd)
- [Objectives / challenges](#objectives--challenges)
- [SOTA updates](#sota-updates)
- [Conclusion](#conclusion)
- [TODOs for week 13](#todos-for-week-13)


---

# Poster

- Poster have been submitted (yay !)
- Kept in mind the note about IO-TCP optimization of TX and ZeroNIC RX for the poster presentation next Wednesday
- Thanks again for your useful reviews

![bg right fit](img/S11/poster.png)

---

# About computational SSD

![bg right fit](img/S11/willow.png)

- More or less the same as accelerator + NVMe-oF
- Write latency improved in one of willow SSD app (*Caching*) because they use it as a custom cache 

---

# About computational SSD (cont.)

- Another SSD app (*Append*) about file system offload
  - If physical length > logical length, then special write issued and updated length logged
  - If not, host-side application invoke the file system
- Not directly linked with file synchronization of IO-TCP but could be a source of inspiration
- [https://www.usenix.org/system/files/conference/osdi14/osdi14-paper-seshadri.pdf](https://www.usenix.org/system/files/conference/osdi14/osdi14-paper-seshadri.pdf)

---

# Objectives / challenges

- IO-TCP
  - Lack of file synchronization
  - File-system change not propagated on NIC stack
- Scalio
  - Batched write efficient for throughput, not for latency
- ZeroNIC
  - Zero-copy limited by number of queues in the NIC

---

# SOTA updates

![bg right fit](img/S11/sota-arch.png)

- Rewrite problems section to architectural challenges
- Clear separation between high level disagregation problems and SOTA problems

---

# SOTA updates

![bg right fit](img/S11/sota-design.png)

- SOTA design and solutions :
  - Added batched write section
  - Added Split stack section

---
# SOTA updates

![bg right fit](img/S11/sota-challenges.png)

- New section about unresolved challenges found in papers
---

# Conclusion

- Poster is done, one deadline eliminated
- Focused on challenges yet to solve in SOTA

---

# TODOs for week 13

- Complete unresolved challenges section in SOTA for other papers

---

# That's all for today !
