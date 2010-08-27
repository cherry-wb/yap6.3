/******************************************************************************\
*                                                                              *
*    SimpleCUDD library (www.cs.kuleuven.be/~theo/tools/simplecudd.html)       *
*  SimpleCUDD was developed at Katholieke Universiteit Leuven(www.kuleuven.be) *
*                                                                              *
*  Copyright Katholieke Universiteit Leuven 2008, 2009, 2010                   *
*                                                                              *
*  Author:       Bernd Gutmann                                                 *
*  File:         problogmath.c                                                 *
*  $Date:: 2010-08-25 15:23:30 +0200 (Wed, 25 Aug 2010)           $            *
*  $Revision:: 4683                                               $            *
*                                                                              *
*                                                                              *
********************************************************************************
*                                                                              *
* Artistic License 2.0                                                         *
*                                                                              *
* Copyright (c) 2000-2006, The Perl Foundation.                                *
*                                                                              *
* Everyone is permitted to copy and distribute verbatim copies of this license *
* document, but changing it is not allowed.                                    *
*                                                                              *
* Preamble                                                                     *
*                                                                              *
* This license establishes the terms under which a given free software Package *
* may be copied, modified, distributed, and/or redistributed. The intent is    *
* that the Copyright Holder maintains some artistic control over the           *
* development of that Package while still keeping the Package available as     *
* open source and free software.                                               *
*                                                                              *
* You are always permitted to make arrangements wholly outside of this license *
* directly with the Copyright Holder of a given Package. If the terms of this  *
* license do not permit the full use that you propose to make of the Package,  *
* you should contact the Copyright Holder and seek a different licensing       *
* arrangement.                                                                 *
* Definitions                                                                  *
*                                                                              *
* "Copyright Holder" means the individual(s) or organization(s) named in the   *
* copyright notice for the entire Package.                                     *
*                                                                              *
* "Contributor" means any party that has contributed code or other material to *
* the Package, in accordance with the Copyright Holder's procedures.           *
*                                                                              *
* "You" and "your" means any person who would like to copy, distribute, or     *
* modify the Package.                                                          *
*                                                                              *
* "Package" means the collection of files distributed by the Copyright Holder, *
* and derivatives of that collection and/or of those files. A given Package    *
* may consist of either the Standard Version, or a Modified Version.           *
*                                                                              *
* "Distribute" means providing a copy of the Package or making it accessible   *
* to anyone else, or in the case of a company or organization, to others       *
* outside of your company or organization.                                     *
*                                                                              *
* "Distributor Fee" means any fee that you charge for Distributing this        *
* Package or providing support for this Package to another party. It does not  *
* mean licensing fees.                                                         *
*                                                                              *
* "Standard Version" refers to the Package if it has not been modified, or has *
* been modified only in ways explicitly requested by the Copyright Holder.     *
*                                                                              *
* "Modified Version" means the Package, if it has been changed, and such       *
* changes were not explicitly requested by the Copyright Holder.               *
*                                                                              *
* "Original License" means this Artistic License as Distributed with the       *
* Standard Version of the Package, in its current version or as it may be      *
* modified by The Perl Foundation in the future.                               *
*                                                                              *
* "Source" form means the source code, documentation source, and configuration *
* files for the Package.                                                       *
*                                                                              *
* "Compiled" form means the compiled bytecode, object code, binary, or any     *
* other form resulting from mechanical transformation or translation of the    *
* Source form.                                                                 *
* Permission for Use and Modification Without Distribution                     *
*                                                                              *
* (1) You are permitted to use the Standard Version and create and use         *
* Modified Versions for any purpose without restriction, provided that you do  *
* not Distribute the Modified Version.                                         *
* Permissions for Redistribution of the Standard Version                       *
*                                                                              *
* (2) You may Distribute verbatim copies of the Source form of the Standard    *
* Version of this Package in any medium without restriction, either gratis or  *
* for a Distributor Fee, provided that you duplicate all of the original       *
* copyright notices and associated disclaimers. At your discretion, such       *
* verbatim copies may or may not include a Compiled form of the Package.       *
*                                                                              *
* (3) You may apply any bug fixes, portability changes, and other              *
* modifications made available from the Copyright Holder. The resulting        *
* Package will still be considered the Standard Version, and as such will be   *
* subject to the Original License.                                             *
* Distribution of Modified Versions of the Package as Source                   *
*                                                                              *
* (4) You may Distribute your Modified Version as Source (either gratis or for *
* a Distributor Fee, and with or without a Compiled form of the Modified       *
* Version) provided that you clearly document how it differs from the Standard *
* Version, including, but not limited to, documenting any non-standard         *
* features, executables, or modules, and provided that you do at least ONE of  *
* the following:                                                               *
*                                                                              *
* (a) make the Modified Version available to the Copyright Holder of the       *
* Standard Version, under the Original License, so that the Copyright Holder   *
* may include your modifications in the Standard Version.                      *
* (b) ensure that installation of your Modified Version does not prevent the   *
* user installing or running the Standard Version. In addition, the Modified   *
* Version must bear a name that is different from the name of the Standard     *
* Version.                                                                     *
* (c) allow anyone who receives a copy of the Modified Version to make the     *
* Source form of the Modified Version available to others under                *
* (i) the Original License or                                                  *
* (ii) a license that permits the licensee to freely copy, modify and          *
* redistribute the Modified Version using the same licensing terms that apply  *
* to the copy that the licensee received, and requires that the Source form of *
* the Modified Version, and of any works derived from it, be made freely       *
* available in that license fees are prohibited but Distributor Fees are       *
* allowed.                                                                     *
* Distribution of Compiled Forms of the Standard Version or Modified Versions  *
* without the Source                                                           *
*                                                                              *
* (5) You may Distribute Compiled forms of the Standard Version without the    *
* Source, provided that you include complete instructions on how to get the    *
* Source of the Standard Version. Such instructions must be valid at the time  *
* of your distribution. If these instructions, at any time while you are       *
* carrying out such distribution, become invalid, you must provide new         *
* instructions on demand or cease further distribution. If you provide valid   *
* instructions or cease distribution within thirty days after you become aware *
* that the instructions are invalid, then you do not forfeit any of your       *
* rights under this license.                                                   *
*                                                                              *
* (6) You may Distribute a Modified Version in Compiled form without the       *
* Source, provided that you comply with Section 4 with respect to the Source   *
* of the Modified Version.                                                     *
* Aggregating or Linking the Package                                           *
*                                                                              *
* (7) You may aggregate the Package (either the Standard Version or Modified   *
* Version) with other packages and Distribute the resulting aggregation        *
* provided that you do not charge a licensing fee for the Package. Distributor *
* Fees are permitted, and licensing fees for other components in the           *
* aggregation are permitted. The terms of this license apply to the use and    *
* Distribution of the Standard or Modified Versions as included in the         *
* aggregation.                                                                 *
*                                                                              *
* (8) You are permitted to link Modified and Standard Versions with other      *
* works, to embed the Package in a larger work of your own, or to build        *
* stand-alone binary or bytecode versions of applications that include the     *
* Package, and Distribute the result without restriction, provided the result  *
* does not expose a direct interface to the Package.                           *
* Items That are Not Considered Part of a Modified Version                     *
*                                                                              *
* (9) Works (including, but not limited to, modules and scripts) that merely   *
* extend or make use of the Package, do not, by themselves, cause the Package  *
* to be a Modified Version. In addition, such works are not considered parts   *
* of the Package itself, and are not subject to the terms of this license.     *
* General Provisions                                                           *
*                                                                              *
* (10) Any use, modification, and distribution of the Standard or Modified     *
* Versions is governed by this Artistic License. By using, modifying or        *
* distributing the Package, you accept this license. Do not use, modify, or    *
* distribute the Package, if you do not accept this license.                   *
*                                                                              *
* (11) If your Modified Version has been derived from a Modified Version made  *
* by someone other than you, you are nevertheless required to ensure that your *
* Modified Version complies with the requirements of this license.             *
*                                                                              *
* (12) This license does not grant you the right to use any trademark, service *
* mark, tradename, or logo of the Copyright Holder.                            *
*                                                                              *
* (13) This license includes the non-exclusive, worldwide, free-of-charge      *
* patent license to make, have made, use, offer to sell, sell, import and      *
* otherwise transfer the Package with respect to any patent claims licensable  *
* by the Copyright Holder that are necessarily infringed by the Package. If    *
* you institute patent litigation (including a cross-claim or counterclaim)    *
* against any party alleging that the Package constitutes direct or            *
* contributory patent infringement, then this Artistic License to you shall    *
* terminate on the date that such litigation is filed.                         *
*                                                                              *
* (14) Disclaimer of Warranty: THE PACKAGE IS PROVIDED BY THE COPYRIGHT HOLDER *
* AND CONTRIBUTORS "AS IS' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES. THE  *
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR  *
* NON-INFRINGEMENT ARE DISCLAIMED TO THE EXTENT PERMITTED BY YOUR LOCAL LAW.   *
* UNLESS REQUIRED BY LAW, NO COPYRIGHT HOLDER OR CONTRIBUTOR WILL BE LIABLE    *
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING IN    *
* ANY WAY OUT OF THE USE OF THE PACKAGE, EVEN IF ADVISED OF THE POSSIBILITY OF *
* SUCH DAMAGE.                                                                 *
*                                                                              *
*         The End                                                              *
*                                                                              *
\******************************************************************************/

#include "problogmath.h"


double sigmoid(double x, double slope) {
  return 1.0 / (1.0 + exp(-x * slope));
}

// This function calculates the accumulated density of the normal distribution
// For details see G. Marsaglia, Evaluating the Normal Distribution, Journal of Statistical Software, 2004:11(4).
double Phi(double x)  {
  double s=x;
  double t=0.0;
  double b=x;
  double q=x*x;
  double i=1;

  // if the value is too small or too big, return
  // 0/1 to avoid long computations
  if (x < -10.0) {
    return 0.0;
  }

  if (x > 10.0) {
    return 1.0;
  }

  // t is the value from last iteration
  // s is the value from the current iteration
  // iterate until they are equal
  while(fabs(s-t) >= DBL_MIN) {
    t=s;
    i+=2;
    b*=q/i;
    s+=b;
  }

  return 0.5+s*exp(-0.5*q-0.91893853320467274178);
}

// integrates the normal distribution over [low,high]
double cumulative_normal(double low, double high, double mu, double sigma) {
  return Phi((high-mu)/sigma) - Phi((low-mu)/sigma);
}

// integrates the normal distribution over [-oo,high]
double cumulative_normal_upper(double high, double mu, double sigma) {
  return Phi((high-mu)/sigma);
}


// evaluates the density of the normal distribution
double normal(double x, double mu,double sigma) {
  double inner=(x-mu)/sigma;
  double denom=sigma*sqrt(2*3.14159265358979323846);
  return exp(-inner*inner/2)/denom;
}

double cumulative_normal_dmu(double low, double high,double mu,double sigma) {
  return normal(low,mu,sigma) - normal(high,mu,sigma);
}

double cumulative_normal_upper_dmu(double high,double mu,double sigma) {
  return  - normal(high,mu,sigma);
}


double cumulative_normal_dsigma(double low, double high,double mu,double sigma) {
  return (((mu-high)*normal(high,mu,sigma) - (mu-low)*normal(low,mu,sigma))/sigma);
}

double cumulative_normal_upper_dsigma(double high,double mu,double sigma) {
  return (mu-high)*normal(high,mu,sigma);
}


// this function parses two strings "$a;$b" and "???_???l$ch$d" where $a-$d are (real) numbers
// it is used to parse in the parameters of continues variables from the input file
density_integral parse_density_integral_string(char *input, char *variablename) {
  density_integral result;
  int i;
  char garbage[64], s1[64],s2[64],s3[64],s4[64];

  if(sscanf(input, "%64[^;];%64[^;]", s1,s2) != 2) {
    fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "The string should contain 2 fields seperated by ; characters.\n");
    exit(EXIT_FAILURE);
  }

  if (IsRealNumber(s1)) {
    result.mu=atof(s1);
  } else {
    fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "%s is not a number\n",s1);
    exit(EXIT_FAILURE);
  }

  if (IsRealNumber(s2)) {
    result.log_sigma=atof(s2);
  } else {
    fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "%s is not a number\n",s2);
    exit(EXIT_FAILURE);
  }

/*  if (result.sigma<=0) { */
/*     fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string",input); */
/*     fprintf(stderr, "The value for sigma has to be larger than 0.\n"); */

/*     exit(EXIT_FAILURE); */
/*   } */

  if (sscanf(variablename,"%64[^lh]l%64[^lh]h%64[^lh]",garbage,s3,s4) != 3) {
    fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string\n",variablename);
    fprintf(stderr, "The string should contain 2 fields seperated by ; characters.\n");
    exit(EXIT_FAILURE);
  }

  //  replace the d by . in s1 and s2
  for(i=0; s3[i]!='\0' ; i++) {
    if (s3[i]=='d') {
      s3[i]='.';
    }
    if (s3[i]=='m') {
      s3[i]='-';
    }
  }
  for(i=0; s4[i]!='\0' ; i++) {
    if (s4[i]=='d') {
      s4[i]='.';
    }
    if (s4[i]=='m') {
      s4[i]='-';
    }
  }

  if (IsRealNumber(s3)) {
    result.low=atof(s3);
  } else {
    fprintf(stderr, "Error at parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "%s is not a number\n",s1);
    exit(EXIT_FAILURE);
  }

 if (IsRealNumber(s4)) {
    result.high=atof(s4);
  } else {
    fprintf(stderr, "Error ar parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "%s is not a number\n",s1);
    exit(EXIT_FAILURE);
  }

  
  if (result.low>result.high) {
    fprintf(stderr, "Error ar parsing the string %s in the function parse_density_integral_string\n",input);
    fprintf(stderr, "The value for low has to be larger than then value for high.\n");
    fprintf(stderr, " was [%f, %f]\n",result.low, result.high);
    fprintf(stderr, " input %s \n",input);
    fprintf(stderr, " variablename %s \n",variablename);
 
    exit(EXIT_FAILURE);
  }


  return result;
}