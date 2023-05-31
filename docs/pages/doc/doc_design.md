---
title: Software Architecture of ECI4FOAM
permalink: doc_design.html
keywords: 
sidebar: doc_design
folder: doc
---

# Software Architecture

The core of the library is the commDataLayer class that stores objects such as scalar, vectors, tensor or other OpenFOAM classes in the Objectregistry. Once an object is registered in the database, it can be retrieved or modified by providing the type of the object and the name of the object. The commDataLayer has the databases: in,out and parameter. Objects can be stored in each of the databases/objectregistries by calling the function storeObj or can be erased and retrieved by calling the corresponding public function.


{% include image.html file="FMU4FOAM.png"  alt="FMU4FOAM Design" caption="Software Architecture of ECI4FOAM" %}


The ECI4FOAM library aims to simplify the data exchange with external software and provides the top level function. externalIOObjectList is similar to the functionObjects implementation and stores all the registered externalIOObjects. To extend the capability of the library a new class derived from externalIOObject can be created. The derived class registers the member in the commDataLayer, so other classes have the ability to retrieve the objects.


{% include links.html %}
