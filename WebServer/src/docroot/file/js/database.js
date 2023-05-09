const tableDbList = document.querySelector('#listdatabases');
const tableTbList = document.querySelector('#listtables');

const formElements = document.forms[0].elements;
const taSql = formElements.sqlRequest;
const txtDoingTbName = formElements.doingtbname;
const txtNamePole = formElements.namepole;
const selectTypePole = formElements.typepole;
const checkBoxPK = formElements.isPK;

function onclick_to_row_list_tables(event){
    var valueCell = event.target.innerText;
    var cellIndex = event.target.cellIndex;
    var nameCell = tableTbList.rows[0].cells[cellIndex].getAttribute("name");
    //console.log(valueCell, nameCell);
    if(nameCell == "table_name"){
        txtDoingTbName.value = valueCell;
    }
    else if(nameCell == "column_name"){
        txtNamePole.value = valueCell;
    }
    else if(nameCell == "data_type"){
        if(valueCell == "int"){
            selectTypePole.value = "int";
        }
        else if(valueCell == "date"){
            selectTypePole.value = "date";
        }
        else{
            selectTypePole.value = "text";
        }
    }
    else if(nameCell == "is_nullable"){
        if(valueCell == "true"){
            checkBoxPK.value = true;
        }
        else{
            checkBoxPK.value = false;
        }
    }
}
function onclick_to_row_list_databases(){
    //console.log('Click! list databases');
}