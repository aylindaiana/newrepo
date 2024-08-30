#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>

using namespace std;

void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
class Fecha{
private:
    int dia,mes, anio;
public:
    int getDia(){return dia;}
    int getMes(){return mes;}
    int getAnio(){return anio;}

    void setDia(int d){dia=d;}
    void setMes(int m){mes=m;}
    void setAnio(int a){anio=a;}

    void Cargar() {
        cout<<"DIA ";
        cin>>dia;
        cout<<"MES "<<"/ ";
        cin>>mes;
        cout<<"ANIO "<<"/ ";
        cin>>anio;
    }
    void Mostrar(){
        cout<<dia<<"/";
        cout<<mes<<"/";
        cout<<anio<<endl;
    }
};
class Final{
private:
    int legajo;
    char nombreApellido[30];
    int DNI;
    Fecha fechaNacimiento;
    float aporteVoluntario;
    int carrera; //1-10
    bool estado;

public:
    Final(){estado=false;}
    int getLegajo(){return legajo;}
    bool getEstado(){return estado;}

    void setLegajo(int _legajo){legajo=_legajo;}
    void setNombreAapellido(const char *na){strcpy(nombreApellido, na);}
    void setDni(int dni){DNI=dni;}
    Fecha setFechaNacimiento(Fecha fn){fechaNacimiento=fn;}
    void setAporteVoluntario(float av){aporteVoluntario=av;}
    void setCarrera(int carrier){carrera=carrier;}
    void setEstado(bool est){estado=est;}
    //reviso
    void Cargar(int leg){
        if(!estado){
            legajo=leg;
            cout<<endl<<"NOMBRE Y APELLIDO: ";
            cin>>nombreApellido;
            cout<<endl<<"DNI: ";
            cin>>DNI;
            cout<<endl<<"APORTE VOLUNTARIO: ";
            cin>>aporteVoluntario;
            cout<<endl<<"CARRERA: ";
            cin>>carrera;
        while(carrera<1 || carrera>10)
        {
            system("cls");
            cout<<" ELIJA UNA CARRERA ENTRE EL 1 Y EL 10 "<<endl;
            cout<<endl<<"CARRERA: ";
            cin>>carrera;
        }
        cout<<"FECHA DE NACIMIENTO"<<endl;
        fechaNacimiento.Cargar();
        estado=true;
        }

    }
    void Mostrar(){
        if(estado){
            cout<<"LEGAJO: "<<legajo<<endl;
            cout<<"NOMBRE Y APELLIDO: "<<nombreApellido<<endl;
            cout<<"DNI: "<<DNI<<endl;
            cout<<"APORTE VOLUNTARIO: "<<aporteVoluntario<<endl;
            cout<<"CARRERA: "<<carrera<<endl;
            fechaNacimiento.Mostrar();
        }
    }
};

class ArchivoFinal{
private:
	char nombre[30];
public:
	ArchivoFinal(const char *n="final.dat"){///cambiar las comillas si no compila!!!!!
		strcpy(nombre, n);
	}
	Final leerRegistro(int pos){
		Final reg;
		//reg.setEstado(false);
		FILE *p;
		p=fopen(nombre, "rb");
		if(p==NULL) return reg;
		fseek(p, sizeof reg*pos,0);
		fread(&reg, sizeof reg,1, p);
		fclose(p);
		return reg;
        }
		int contarRegistros(){
			FILE *p;
			p=fopen(nombre, "rb");
			if(p==NULL) return -1;
			fseek(p, 0,2);
			int tam=ftell(p);
			fclose(p);
			return tam/sizeof(Final);
		}
        bool modificar(Final obj, int pos){
            bool cambio;
            FILE *p;
            if(p==NULL){return false;}
            p = fopen(nombre,"rb+");
            fseek(p, sizeof(obj) * pos, SEEK_SET);
            cambio= fwrite(&obj, sizeof(Final), 1, p);
            fclose(p);
            return cambio;
    }
        bool validarID(int leg)
        {
            Final reg;
            FILE *p;
            p = fopen(nombre,"rb");
            if (p == NULL) return false;
            while (fread(&reg, sizeof reg, 1, p)==1 ){
                if (reg.getLegajo() == leg)
                {
				return true;
                }
            }
            fclose(p);
            return false;
    }
        int buscarPosicion(int leg){
            Final reg;
            FILE *p;
            int pos = 0;
            p = fopen(nombre,"rb");
            if (p == NULL) return -2;
            while (fread(&reg, sizeof(Final),1,p)){
                if (reg.getLegajo() == leg)
                {
                    fclose(p);
                    return pos;
                }
                pos++;
            }
            fclose(p);
            return -1;
    }
        void listarArchivo() {
            Final reg;
            FILE* p;
            p = fopen(nombre, "rb");
            if (p == NULL) {
                cout << "No se pudo abrir el archivo." << endl;
                return;
            }
            while (fread(&reg, sizeof reg, 1, p) == 1) {
                reg.Mostrar();
                cout << endl;
            }
            fclose(p);

        }

        bool grabarRegistro(Final reg){
            FILE *p;
            p=fopen(nombre, "ab");
            if(p==NULL) return false;
            bool escribio=fwrite(&reg, sizeof reg,1, p);
            fclose(p);
            return escribio;
        }
};
void darDeAlta(){
    Final reg;
    ArchivoFinal archreg;
    int leg;
    cout<<"LEGAJO: ";
    cin>>leg;
    while(archreg.validarID(leg)){
        system("cls");
        cout<<"EL LEGAJO INGRESADO YA EXISTE INGRESE OTRO"<<endl;
        cout<<"LEGAJO: ";
        cin>>leg;
    }
    reg.Cargar(leg);
    archreg.grabarRegistro(reg);
    cout<<"SU LEGAJO SE GUARDO CORRECTAMENTE"<<endl;
    system("pause");
}
void darDeBaja(){
    Final reg;
    ArchivoFinal archreg;
    int leg;
    cout<<"LEGAJO A DAR DE BAJA: ";
    cin>>leg;
    int pos = archreg.buscarPosicion(leg); ///me fijo si existe el codigo
    while(!archreg.validarID(leg)){
        system("cls");
        cout<<"EL LEGAJO NO EXISTE INGRESE OTRO"<<endl;
        cout<<"LEGAJO A DAR DE BAJA: "<<endl;
        cin>>leg;

    }
    if (pos==-1){
        cout<<"EL REGISTRO YA NO EXISTE"<<endl;
        return;
    }
    if(pos==-2){
        cout<<"EL ARCHIVO TIENE UN ERROR"<<endl;
        return;
    }
    reg = archreg.leerRegistro(pos);
    if(reg.getEstado()==false){
        cout<<"USTED YA DIO DE BAJA ESTE LEGAJO"<<endl;
        system("pause");
        return;
    }
    reg.setEstado(false);
    if(archreg.modificar(reg, pos)){
        cout<<"SE ELIMINO CON EXITO"<<endl;
    }else{
        cout<<"HUBO UN ERROR"<<endl;
    }

    system("pause");
}


void Entrada(){
    system("cls");
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"BIENVENIDO A SUS REGISTROS"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    cout<<"OPCION 1 - DAR DE ALTA "<<endl;
    cout<<"OPCION 2 - DAR DE BAJA"<<endl;
    cout<<"OPCION 3 - LISTAR REGISTROS"<<endl;
    cout<<"OPCION 4 - SALIR"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;

}

int main(){
    ArchivoFinal reg;
    int option;
    bool salida=true;
    SetColor(11);

    while(salida){
        Entrada();
        cout<<"Ingrese su opcion: ";
        cin>>option;
        while(option<1 || option>4){
        Entrada();
        cout<<"Ingrese su opcion: ";
        cin>>option;
        }
        switch(option){
        case 1:
            system("cls");
            darDeAlta();
            break;
        case 2:
            system("cls");
            darDeBaja();
            break;
        case 3:
            system("cls");
            reg.listarArchivo();
            system("pause");
            break;
        case 4:
            salida=false;
            break;
            system("pause");

        }
    }

return 0;
}
