#ifndef _BERTHAINGEN_MOLECULE_INC_
#define _BERTHAINGEN_MOLECULE_INC_

#include <string>
#include <vector>

#include "ptable.hpp"

namespace berthaingen
{
   // maybe I need to adda 3D point class 
   class atom
   {
     private:
   
       int charge_;
       double x_, y_, z_;
       ptable::element e_;
   
       void reset_ ()
       {
         charge_ = 0;
         x_ = y_ = z_ = 0.0d;
         e_ = ptable::NO_ELEMENT;
       }
   
     public:
   
       atom()
       {
         reset_ ();
       };

       atom(double, double, double, const char *);
       
       ~atom()
       {
         reset_ ();
       };

       void translate (double, double, double);
   
       void set_x(double in)
       {
         x_ = in;
       };
   
       void set_y(double in)
       {
         y_ = in;
       };
   
       void set_z(double in)
       {
         z_ = in;
       };
   
       void set_symbol(const char * s)
       {
         e_ = ptable::symbol_to_element(s);
       };

       void set_symbol(ptable::element e)
       {
         e_ = e;
       };

       void set_charge (int in)
       {
         charge_ = in;
       };

       int get_charge() const
       {
         return charge_;
       };
   
       double get_x() const
       {
         return x_;
       };
   
       double get_y() const
       {
         return y_;
       };
   
       double get_z() const
       {
         return z_;
       };

       ptable::element get_element() const
       {
         return e_;
       };
   
       std::string get_symbol() const
       {
         std::string symbol = ptable::atomic_symbol (e_);
         return symbol;
       };

       inline friend std::ostream & operator<< (std::ostream & os, const atom & a)  
       { 
         os << a.get_symbol() << " " 
            << a.get_x() << " "
            << a.get_y() << " "
            << a.get_z() << " "
            << a.get_charge();

         return os;  
       }
   };

   class bond
   {
     public:
       enum bond_type {
         NOBOND,
         SINGLE,
         DOUBLE,
         TRIPLE
       };

     private:
       bond_type type_;
       int aidx1_, aidx2_;

       void reset_ ()
       {
         aidx1_ = -1;
         aidx2_ = -1;
         type_ = bond_type::NOBOND;
       }
   
     public:
   
       bond()
       {
         reset_ ();
       };

       bond (int, int, bond_type);
       
       ~bond()
       {
         reset_ ();
       };
   
       void set_type(int);

       void set_type(bond_type in)
       {
         type_ = in;
       };

       void set_a1(int in)
       {
         aidx1_ = in;
       };

       int get_a1() const
       {
         return aidx1_ ;
       };

       void set_a2(int in)
       {
         aidx2_ = in;
       };

       int get_a2() const
       {
         return aidx2_;
       };

       int get_type() const;
       
       inline friend std::ostream& operator<<(std::ostream & os, const bond & b)
       {
         os << b.get_a1() << " " 
            << b.get_a2() << " " 
            << b.get_type();
         
         return os;  
       }
   };
   
   class molecule 
   {
     private:
       std::string name_;
       std::vector<atom> atoms_;
       std::vector<bond> bonds_;
 
       void reset_()
       {
         name_ = "";
         atoms_.clear();
         bonds_.clear();
       };

        void copy_ (const molecule &);
   
     public:
   
       molecule() 
       {
         reset_ ();
       };

       molecule(const molecule &);
       
       ~molecule()
       {
         reset_ ();
       };

       molecule & operator= (const molecule &); 

       void center (double, double, double);

       const atom & get_atom (int idx) const
       {
         return atoms_[idx];
       };

       std::vector<atom>::const_iterator get_atoms_begin () const
       {
         return atoms_.begin();
       };

       std::vector<atom>::const_iterator get_atoms_end () const
       {
         return atoms_.end();
       };

       std::vector<bond>::const_iterator get_bonds_begin () const
       {
         return bonds_.begin();
       };

       std::vector<bond>::const_iterator get_bonds_end () const
       {
         return bonds_.end();
       };

       const std::string & get_molname () const
       {
         return name_;
       };

       void set_molname (const char * in) 
       {
         name_ = in;
       };

       const std::vector<atom> & get_atomlist () const
       {
         return atoms_;
       };

       unsigned int get_atomsize () const
       {
         return atoms_.size();
       };

       const std::vector<bond> & get_bondlist () const
       {
         return bonds_;
       };

       unsigned int get_bondsize () const
       {
         return bonds_.size();
       };

       void add_atom (const atom & a)
       {
         atoms_.push_back(a);
       }

       void add_bond (const bond & b)
       {
         bonds_.push_back(b);
       }

       void add_fragment (const molecule & );

       bool read_xyz_file (const char *, bool antoau = false);

       void get_xyzfile (std::ostream & os);
       
       inline friend std::ostream & operator<< (std::ostream & os, const molecule & m)
       {
         os << "Atoms: " << m.get_atomsize() << std::endl;
         std::vector<atom>::const_iterator aiter = m.get_atomlist().begin();
         for (; aiter != m.get_atomlist().end(); ++aiter)
           os << aiter->get_symbol() << " " 
              << aiter->get_x()  << " "
              << aiter->get_y()  << " "
              << aiter->get_z()  << " " 
              << aiter->get_charge() << std::endl;
         
         os << "Bonds: " << m.get_bondsize() << std::endl;
         std::vector<bond>::const_iterator biter = m.get_bondlist().begin();
         for (; biter != m.get_bondlist().end(); ++biter)
           os << biter->get_a1() << " " 
              << biter->get_a2() << " " 
              << biter->get_type() << std::endl;
                 
         return os;  
       };
    };
}

#endif
