# Poster notes

## Context

- End of Moore’s Law and Dennard scaling has stalled CPU capacity while I/O device increases rapidly.  
- CPU spend up to 70 % of cycles for I/O.
- Resource stranding lead from 10 % to 30 % wastes of server resources

## Motivation

- Disaggregated hardware from the host to save CPU cycles.
- GPU and memory are heavily-studied
- SmartNIC-to-NVMe access needs further exploration.

## Objectives

Modern solutions mainly lack of file synchronization [1] or write operations efficiency [2]. (this part centered and other to the left)
Our goal is to :

- Resolve file synchronization 
- Maximize throughput, minimize latency in write operations

## Approach

- Direct storage access from smartNIC to SSDs
- Direct access over the network
- Hardware-accelerated encryption for secure transfer
- Performance evaluation

## State-of-the-art

Draw a table with IO-TCP, Scalio and ZeroNIC
