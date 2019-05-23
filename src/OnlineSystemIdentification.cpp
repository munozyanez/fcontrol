#include "OnlineSystemIdentification.h"

OnlineSystemIdentification::OnlineSystemIdentification()
{

    //Default params: order 1, forgetting factor 0.98
    OnlineSystemIdentification(1,0.98);

}

OnlineSystemIdentification::OnlineSystemIdentification(long new_numOrder, long new_denOrder, double new_ff)
{
    ff=new_ff;

    denOrder= new_denOrder;
    numOrder= new_numOrder;

    if(denOrder < numOrder)
    {
        numOrder = denOrder;
        cout << "Only causal systems!!! Reducing numerator to order: " << numOrder << endl;
    } //now it is causal, numOrder <= denOrder

    //share orders available, allowing den use numerator unused orders.
    if( denOrder + numOrder > rlms_N)
    {
        if (numOrder < rlms_N/2) //Give num orders to den.
        {
        denOrder = rlms_N - numOrder;
        cout << "Maximum order reached. Reducing orders to: num = " << numOrder << ", den =" << denOrder << endl;
        }
        else //The only possibility is to reduce both to maximum size.
        {
            numOrder = rlms_N/2;
            denOrder = numOrder;
            cout << "Maximum order reached. Reducing orders to: num = " << numOrder << ", den = " << denOrder << endl;


        }
    }

    order = numOrder + denOrder;

//    P.resize(order,NoChange); //no longer needed
    P=P.Random(order,order);
    cout << "--> Initial P <--" << endl << P << endl;
//    phi.resize(order,order); //no longer needed
    phi=phi.Random(order,1);
    cout << "--> Initial phi <--" << endl << phi << endl;
//    L.resize(order,NoChange);//no longer needed
    L=L.setZero(order,1);
    cout << "--> Initial L <--" << endl << L  << endl;

    th=th.setZero(order,1);
    cout << "--> Initial th <--" << endl << th << endl << "--------------order : " << order << endl;


}

long OnlineSystemIdentification::UpdateSystem(double input, double output)
{

ti++;
    //move all phi input data one position backwards to have inputs from actual to last needed past values.
    for (int i=numOrder-1; i>0; i--) //Leave the first place empty for current input value
    {
        //input indexes can start from zero to include actual input??
        phi(i+denOrder) = phi(i+denOrder-1);
    }
    phi(0+denOrder)=input;

    //        cout << "b0 " << (out[ti]-phi.transpose()*th)/in[ti] << ", at step: " <<  ti << endl;
    //        cout << "phi: " << phi.transpose() << endl;

    L = (P*phi)/(ff+(phi.transpose()*P*phi));
//    cout << "L " << L.transpose() << ", at step: " <<  ti << endl;

    P=(1/ff)*(P-L*phi.transpose()*P);
//    cout << "P " << P << ", at step: " <<  ti << endl;

    err=output-phi.transpose()*th;
//    cout << "Error preditcion " << err << ", at step: " <<  ti << endl;

    th=th+(L*err);
//    cout << "Params " << th.transpose() << endl;

    //move all phi output data one position backwards for the next iteration
    for (int i=denOrder-1; i>0; i--)
    {
        phi(i) = phi(i-1);
    }
    //and add the actual value
    phi(0)=output;

    return 0;

}

long OnlineSystemIdentification::GetZTransferFunction(vector<double> &num, vector<double> &den)
{

    for (int i=0; i<numOrder; i++)
    {
        num[i]=th(i);
//        cout << "num i= " << i << " num: " << num[i] << " th: " << th(i)<<endl;
    }

    for (int i=0; i<denOrder; i++)
    {
        den[i]=th(i+numOrder);
//        cout << "den i= " << i << " den: " << den[i] << " th: " << th(i+numOrder)<<endl;
    }
    return 0;
}

vector<double> OnlineSystemIdentification::GetParamsVector()
{
    vector<double> params(order);
    for (int i=0; i<order; i++)
    {
        params[i]=th(i);
    }
    return params;
}