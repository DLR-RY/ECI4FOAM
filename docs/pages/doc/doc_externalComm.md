---
title: External Communication
permalink: doc_externalComm.html
keywords: design
sidebar: doc_sidebar
folder: doc
---

The main concept of the externalComm library is similar to the FMI Standard and it can be viewed as blackbox with time-dependent input and output values and parameters that are fixed in time.

## Usage


### add to OpenFOAM
add following library to the controlDict

```
libs(externalComm)
```

### input

```cpp
    // create new entry
    const Time& runTime = mesh_.time(); // we need the singleton time
    commDataLayer& data = commDataLayer::New(runTime);

    scalar varName = "varName";
    scalar varValue = "varValue";

    data.storeObj(varValue,varName,commDataLayer::causality::in);
```

```cpp
    // work with data
    commDataLayer& data = commDataLayer::New(runTime);
    // get varName from registry
    scalar& val = data.getObj<scalar>("varName",commDataLayer::causality::in);
    val = newValue;
```

### output

```cpp
    // create new entry
    const Time& runTime = mesh_.time(); // we need the singleton time
    commDataLayer& data = commDataLayer::New(runTime);

    scalar varName2 = "varName2"; 
    
    vector varValue2(0,0,0); 
    data.storeObj(varValue2,varName2,commDataLayer::causality::out);
```

```cpp
    // work with data
    commDataLayer& data = commDataLayer::New(runTime);
    // get varName2 from registry
    vector& val2 = data.getObj<vector>("varName2",commDataLayer::causality::out); 
    val2 = newValue;
```

## Design

As hinted above the approach is similar to FMI standard with the categorization of variables in 3 causality:

- input -> changes in time (and space)
- output -> changes in time (and space)
- parameter -> constant in time (and space)





### example