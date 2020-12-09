# Laboratorul 07 - Modelul Replicated Workers

## Concepte

- ExecutorService
- ForkJoinPool

## Logica de baza
```
while (1)  
    ia un task din pool  
    execută task-ul  
    adaugă zero sau mai multe task-uri rezultate în pool  
```