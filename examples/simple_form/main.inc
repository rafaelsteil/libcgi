        <td valign="Top" bgcolor="#ffffff">
      <table cellpadding="2" cellspacing="2" border="0" align="Center" width="80%">
	<form action="form2.cgi" method="get">
        <tbody>
          <tr>
            <td valign="Top" width="30%"><font face="Helvetica, Arial, sans-serif" size="1"> Name:</font></td>
            <td valign="Top"><input type="text" name="name" size="20"></td>
          </tr>
          <tr>
            <td valign="Top" width="30%"><font face="Helvetica, Arial, sans-serif" size="1">Age:</font></td>
            <td valign="Top"><input type="text" name="age" size="10"></td>
          </tr>
          <tr>
            <td valign="Top" width="30%"><font face="Helvetica, Arial, sans-serif" size="1">Gender:</font></td>
            <td valign="Top"><select name="gender">
		<option value="Unknown" selected>-- Choose one --</option>
		<option value="Male">Male</option>
		<option value="Female">Female</option>
		</select>
	</td>
          </tr>
          <tr>
            <td valign="Top" width="30%"><font face="Helvetica, Arial, sans-serif" size="1">
Radio button 1:</font></td>
            <td valign="Top"><font face="helvetica, arial, sans-serif" size="1">
	<input type="radio" name="radiobutton1" value="yes" checked>Yes
	&nbsp;&nbsp;&nbsp;
	<input type="radio" name="radiobutton1" value="no">No
	</font>
	</td>
          </tr>
          <tr>
            <td valign="Top" width="30%"><font face="Helvetica, Arial, sans-serif" size="1">Radio button 2:</font>
            </td>
            <td valign="Top">
		<font face="helvetica, arial, sans-serif" size="1">
		<input type="radio" name="radiobutton2" value="yes" checked>Yes
		&nbsp;&nbsp;&nbsp;
		<input type="radio" name="radiobutton2" value="no">No
		</font>
            </td>
          </tr>
          <tr>
            <td valign="Top" colspan="2" align="center"><input type="submit" name="action" value=" Click to submit">
            </td>
          </tr>
        </tbody>
	</form>
	main.inc
      </table>
