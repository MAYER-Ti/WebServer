const tableDbList = document.querySelector('#listdatabases');
const tableTbList = document.querySelector('#listtables');

const formElements = document.forms[0].elements;
const taSql = formElements.sqlRequest;
const txtDoingTbName = formElements.doingtbname;
const txtNamePole = formElements.namepole;
const selectTypePole = formElements.typepole;
const checkBoxPK = formElements.isPK;

//console.log(rowForm);

//var inputform = inputdiv.firstElementChild;
//inputform.setAttribute("name", "nameinputform");




function ShowInputForms(nameSearchTable, column, row){
    console.log(row.children.length);
    if(row.children.length != 0){

        if(document.querySelectorAll("#buttonInputRowForm").length != 0){
            document.querySelector("#buttonInputRowForm").remove();
        }
        row.innerHTML = "";
    }
    var button = document.createElement("button");
    button.setAttribute("id", "buttonInputRowForm");
    button.setAttribute("class", "w-100 btn btn-lg btn-success");
    button.setAttribute("value", "append_row");
    button.innerText = "Append row";
    row.after(button);
    var countOfColumns = CountColumnsOfTable(nameSearchTable).length;
    var inputdiv;
    console.log(countOfColumns);
    var arColumnNames = CountColumnsOfTable(nameSearchTable);
    for(i = 0; i < countOfColumns; i++){
        var newColumn = column.cloneNode(true);
        newColumn.firstChild.firstChild.setAttribute("name", ("input"/* + i*/));
        newColumn.firstChild.lastChild.innerText = arColumnNames[i];
        console.log(newColumn);
        row.prepend(newColumn);
    }
    rowForm.setAttribute("visibility", "visible");
}

function onclick_to_row_list_tables(event){
    var valueCell = event.target.innerText;
    var cellIndex = event.target.cellIndex;
    var nameCell = tableTbList.rows[0].cells[cellIndex].getAttribute("name");

    if(nameCell == "table_name"){
        txtDoingTbName.value = valueCell;
        taSql.value = "SELECT * FROM " + valueCell + ";";

        const rowForm = document.querySelectorAll("#rowForm")[0];
        //rowForm.setAttribute("visibility", "hidden");
        const column = document.createElement("div");
        column.setAttribute("class", "col");
        column.innerHTML = '<div class="form-floating"><input type="text" class="form-control" placeholder="column"><label>Name column</label></div>';

        ShowInputForms(valueCell, column, rowForm);
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

function CountColumnsOfTable(nameSearchTable){
    var rows = document.querySelectorAll("#listtables tr");
    console.log(rows);
    var columnsText;
    var listColumnsOfTable = [];
    console.log(nameSearchTable);
    for(i = 0; i < rows.length; i++){
        columnsText = rows[""+i].outerText.split('\t');
        if(columnsText[0] == nameSearchTable){
            listColumnsOfTable.push(columnsText[1]);
            console.log(listColumnsOfTable);
        }
        console.log(columnsText);
    }
    return listColumnsOfTable;
}
