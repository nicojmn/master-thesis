---
marp: true
theme: default
title: "Master's thesis meeting : week 3"
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

## Master's thesis meeting : week 3

### Nicolas Jeanmenne

---
## Table of contents






<div class="container">

<div class="col">

- [Summary from previous week](#summary-from-previous-week)
  - [Results](#results)
- [`basic-cp` updates](#basic-cp-updates)
  - [Example](#example)
  - [`O_DIRECT` flag](#o_direct-flag)
- [Write and read time executions](#write-and-read-time-executions)

</div>

<div class="col">

- [`queue-cp`](#queue-cp)
  - [Modifications in code](#modifications-in-code)
- [Conclusion](#conclusion)
- [TODOs for week 4](#todos-for-week-4)

</div>
</div>

---

# Summary from previous week

- Wrote code to read / write string and files from bdev layer
- Easiest way possible
- Inspiration straight from Intel's code


---

## Results

- 3 binaries : `get_info`, `rwstring` and `basic-cp`
- `basic-cp` is the most interesting program
  - Write an arbitrary file to a bdev
  - Read it back

---

# `basic-cp` updates

- Added the output flag 
- Bdev content copied wherever you want

---

## Example 

![bg right fit](img/s3/basic-cp-arch-example.png)

- File for test : archlinux iso (~1.5GB)


---

## Example 

![bg vertical fit](img/s3/archlinux-fs-out.png)
![bg right fit](img/s3/checksums.png)

- Content is correctly written in output path
- Identicals checksums in lower picture

---

## `O_DIRECT` flag

- `O_DIRECT` : tells to do File I/O to/from user space
- Used without `O_SYNC`, we don't actually get the file on the fs
- Both options don't improve performances

---


# Write and read time executions

- Something bothered me while I was running program
- For big files, write and read performances are pretty similar
- For string and small files, read is $>1000$ times slower than write

---

# `queue-cp`

- Improvement of `basic-cp`
- Instead of sending one I/O request, split it in a queue
- Still WIP

---

## Modifications in code

![bg vertical fit](img/s3/io_task.png)
![bg right fit](img/s3/context-modif.png)
![bg right fit](img/s3/for-loop.png)

- New struct to manage each I/O task
- Store number of operations completed and cumulative time in context struct
- Split read / write by queue depth

---

# Conclusion

- This week was mostly about finding flaws and optimizing `basic-cp`
- Thought it was more or less necessary before moving on blobstore


---

# TODOs for week 4

- KV store implementation
- 
  - Goal : persistent superblock and store
- ...

---

# That's all for today !
