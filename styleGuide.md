# **Shadercore Engine Style Guide**

## **1. File Structure & Organization**

### **1.1. Includes**
- Any file using OpenGL dependencies **must** include headers in this order:  
  ```cpp
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
  #include <GL/gl.h>
  ```
- Always use **absolute include paths** for project headers.

### **1.2. Directory Structure**
```
/project_root
├── include
│   ├── engines
│   │   ├── [EngineName]
│   │   │   ├── engine.hpp
│   │   │   ├── [ModuleName].hpp
│   ├── modules
│       ├── [ModuleName].hpp
├── src
│   ├── engines
│   │   ├── [EngineName]
│   │   │   ├── engine.cpp
│   │   │   ├── [ModuleName].cpp
│   ├── modules
│       ├── [ModuleName].cpp
```

### **1.3. File Comments**
- Every header must have a **header comment** with:
  - **File name**
  - **Parent engine/module**
  - **Brief description**
- Format:
  ```cpp
  /*
  -- [[Name]] of [[Parent]] --

  - Responsibility 1
  - Responsibility 2
  - Responsibility 3
  ...
  */
  ```

---

## **2. Engine & Module Design**

### **2.1. Engines**
- **Engines** manage state and **rarely** interact with other engines via **crates**.
- Must contain:
  - `init(const Crate&)`
  - `update()`
  - `buildCrate(const Crate&) const`
  - `applyCrate(const Crate&)` *(rarely used except for full-system updates)*
- Engines interact **only through crates**, except for:
  - `WindowEngine`
- Engines **must not** access other engines' modules.

### **2.2. Modules**
- **Modules** exist **only** inside their parent engine.
- Modules **must not** interact with each other.
- Must contain:
  - `init()`
  - `update()`
- Stored as **private members** inside their parent engine.

### **2.3. Engine Initialization**
- The constructor does nothing, and is only there to initialize the module at start.
- **Crates** are passed during `init()` but **not** in the constructor.
- The init module is a eqivialent to a normal constructor.

---

## **3. Naming Conventions**

### **3.1. Classes & Structs**
- Use **PascalCase** for all classes and structs.
- Naming format:
  - `RenderEngine`
  - `ShaderModule`
  - `RenderCrate`

### **3.2. Methods & Members**
- Use **camelCase** for class methods and members.
  ```cpp
  void myMethod();
  int myMember;
  ```

### **3.3. File Names**
- Use **camelCase** for file names.

---

## **4. Code Style**

### **4.1. Function Structure**
- **Long functions** are preferred.
- Splitting into smaller functions is allowed but should be **carefully considered**.

### **4.2. Code Order**
1. **Builders**
2. **Setters & Getters** *(preferably one-liners)*
3. **Main Logic**

### **4.3. Comment Separators**
- Use:
  ```cpp
  ////// [[ Section Name ]] //////
  ```

### **4.4. Include Order**
1. OpenGL  
2. GLFW  
3. Vmml
4. Standard Libraries  
5. Project includes  

---

## **5. Best Practices & Habits**
- **Use `const` and references** wherever possible.
- **Avoid `static`** unless necessary.
- **Use `#pragma once`** in headers.
- **Engines & modules must be documented in Excalidraw**.

---
