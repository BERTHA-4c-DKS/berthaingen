#include <string>
#include <vector>

namespace berthaingen
{
   class atom
   {
     private:
   
       float x_, y_, z_;
       std::string symbol_;
   
       void reset_ ()
       {
         x_ = y_ = z_ = 0.0d;
         symbol_ = "";
       }
   
     public:
   
       atom()
       {
         reset_ ();
       };

       atom(float, float, float, const char *);
       
       ~atom()
       {
         reset_ ();
       };
   
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
         symbol_ = s;
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
   
       std::string get_symbol() const
       {
         return symbol_;
       };
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
       
       ~bond()
       {
         reset_ ();
       };
   
       void set_type(int);

       void set_type(bond_type in)
       {
         type_ = in;
       };
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

       bool read_file (const char *);

       std::vector<atom> get_atomlist () const;
       std::vector<bond> get_bondlist () const;
   };
}
