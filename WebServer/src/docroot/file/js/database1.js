const tableDbList = document.querySelector('#listdatabases');
const tableTbList = document.querySelector('#listtables');

const formElements = document.forms[0].elements;
const taSql = formElements.sqlRequest;
const txtDoingTbName = formElements.doingtbname;
const txtNamePole = formElements.namepole;
const selectTypePole = formElements.typepole;
const checkBoxPK = formElements.isPK;

function ShowInputForms(nameSearchTable, column, row){
    if(row.children.length != 0){
        if(document.querySelectorAll("#buttonInputRowForm").length != 0){
            document.querySelector("#buttonInputRowForm").remove();
        }
        row.innerHTML = "";
    }
    var button = document.createElement("button");
    button.setAttribute("class", "w-100 btn btn-lg btn-success");
    button.setAttribute("id", "buttonInputRowForm");
    button.setAttribute("name", "typedo");
    button.setAttribute("value", "append_row");
    button.innerText = "Append row";
    row.after(button);
    var countOfColumns = CountColumnsOfTable(nameSearchTable).length;
    var arColumnNames = CountColumnsOfTable(nameSearchTable);
    for(i = 0; i < countOfColumns; i++){
        var newColumn = column.cloneNode(true);
        newColumn.firstChild.firstChild.setAttribute("name", ("input"));
        newColumn.firstChild.lastChild.innerText = arColumnNames[i];
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
        const column = document.createElement("div");
        column.setAttribute("class", "col");
        column.innerHTML = 
        '<div class="form-floating">'
            '<input type="text" class="form-control" placeholder="column">'
            '<label>Name column</label>'
        '</div>';
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
function onclick_to_row_list_databases(event){
    var clickedUser = event.target.innerText;
    formElements.userName.value = clickedUser;
    var fildsuser = formElements.doinguser;
    for(i = 0; i < fildsuser.length; i++)
        fildsuser[i].value = clickedUser;
}

function CountColumnsOfTable(nameSearchTable){
    var rows = document.querySelectorAll("#listtables tbody tr");
    console.log(rows);
    var columns;
    var listColumnsOfTable = [];
    for(i = 0; i < rows.length; i++){

        columns = rows[i].children; //.outerText.split('\t')
        console.log(columns);
        if(columns[0].innerText == nameSearchTable){
            listColumnsOfTable.push(columns[1].innerText);
        }
    }
    return listColumnsOfTable;
}

function onclick_to_remove_row(event){
    var row = event.target.parentNode.parentNode.parentNode.parentNode;
    var idColumn = row.querySelectorAll('td')[0];
    var inputIdRow = document.createElement("input");
    inputIdRow.setAttribute("type", "hidden");
    inputIdRow.setAttribute("name", "idRowToDelete");
    inputIdRow.setAttribute("value", idColumn.textContent);
    var inputTypeDo = document.createElement("input");
    inputTypeDo.setAttribute("type", "hidden");
    inputTypeDo.setAttribute("name", "typedo");
    inputTypeDo.setAttribute("value", "delete_row");
    var form = document.getElementById("mainForm");
    form.appendChild(input);
    form.appendChild(input2);
    form.submit();
}

function onclick_to_edit_row(event){
    var rowEdit = document.querySelectorAll("#rowFormEdit")[0];
    if(rowEdit.children.length != 0){
        if(document.querySelectorAll("#buttonEditRowForm").length != 0){
            document.querySelector("#buttonEditRowForm").remove();
        }
        rowEdit.innerHTML = "";
    }
    var row = event.target.parentNode.parentNode.parentNode;
    var headerRow = document.querySelectorAll("#listresponse")[0].querySelectorAll('tr')[0];
    var columns = row.querySelectorAll('td');
    var idRow = columns[0].innerText;
    var headerColumns = headerRow.querySelectorAll('td');
    for(i = 0; i < columns.length-1; i++){
        //create input
        var fild = document.createElement("input");
        fild.setAttribute("type", "text");
        fild.setAttribute("class", "form-control")
        fild.setAttribute("name", "editFild");
        fild.setAttribute("value", columns[i].innerText);
        //create label
        var label = document.createElement("label");
        label.innerText = headerColumns[i].innerText;
        //set input and label to div and set it to row
        const column = document.createElement("div");
        column.setAttribute("class", "form-floating");
        column.append(fild);
        column.append(label);
        rowEdit.prepend(column);
        //create hidden inputs to send list of name columns
        var hiddenInputHeader = document.createElement("input");
        hiddenInputHeader.setAttribute("type", "hidden");
        hiddenInputHeader.setAttribute("name", "headerEditFild");
        hiddenInputHeader.setAttribute("value", headerColumns[i].innerText);
        rowEdit.prepend(hiddenInputHeader);
    }
    //create hidden input to send id row
    var hiddenInputIdRow = document.createElement("input");
    hiddenInputIdRow.setAttribute("type", "hidden");
    hiddenInputIdRow.setAttribute("name", "idRowEdit");
    hiddenInputIdRow.setAttribute("value", idRow);
    rowEdit.prepend(hiddenInputIdRow);
    //create button
    var button = document.createElement("button");
    button.setAttribute("class", "w-100 btn btn-lg btn-success");
    button.setAttribute("id", "buttonEditRowForm");
    button.setAttribute("name", "typedo");
    button.setAttribute("value", "edit_row");
    button.innerText = "Edit row";
    rowEdit.after(button);
}

