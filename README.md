# optRANN 
[![Release Version](https://img.shields.io/github/release/jefferis/optRANN.svg)](https://github.com/jefferis/optRANN/releases/latest) 
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/optRANN)](https://CRAN.R-project.org/package=optRANN) 
[![Build Status](https://travis-ci.org/jefferis/optRANN.svg)](https://travis-ci.org/jefferis/optRANN)
 [![Downloads](http://cranlogs.r-pkg.org/badges/optRANN?color=brightgreen)](http://www.r-pkg.org/pkg/optRANN)

Finds the k nearest neighbours for every point in a given dataset
in O(N log N) time using Arya and Mount's ANN library (v1.1.3). There is
support for approximate as well as exact searches, fixed radius searches
and bd as well as kd trees.

This package implements nearest neighbors for the Euclidean (L2) metric.
For the Manhattan (L1) metric, install the [optRANN1](https://github.com/jefferis/optRANN/tree/master-L1) package.

For further details on the underlying ANN library, see http://www.cs.umd.edu/~mount/ANN.

## Installation
### Released versions
The recommendation is to install the released version from [CRAN](https://cran.r-project.org/) by doing:

```r
install.packages("optRANN")
```

### Bleeding Edge
You can, however, download the [tar ball](https://github.com/jefferis/optRANN/tarball/master), and run `R CMD INSTALL` on it, or use the **devtools** package to install the development version:

```r
# install.packages("devtools")

devtools::install_github("jefferis/optRANN")
```

Note: Windows users need [Rtools](http://www.murdoch-sutherland.com/Rtools/) and [devtools](https://CRAN.R-project.org/package=devtools) to install this way.

## Feedback
Please feel free to:

* submit suggestions and bug-reports at: <https://github.com/jefferis/optRANN/issues>
* send pull requests after forking: <https://github.com/jefferis/optRANN/>
* e-mail the maintainer: <jefferis@gmail.com>

## Copyright and License
see [inst/COPYRIGHT](inst/COPYRIGHT) and [DESCRIPTION](DESCRIPTION) files for copyright and license information.
