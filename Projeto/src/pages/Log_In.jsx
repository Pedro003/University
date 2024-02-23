import Header from "../components/Header";
import SignUpForm from "../components/SignUpForm";

/*
Vai conter uma forma de registro (Henrique the builder)
*/
function Log_In(){
    return (
        <>
        <Header/>
        <div className="SignUp">
            <SignUpForm/>
        </div>
        </>
    );
}


export default Log_In