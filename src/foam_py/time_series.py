from foam_py import volScalarField, fvMesh, Vector
import foam_py
import numpy as np
from typing import Protocol, List, Any, Callable
from pathlib import Path
import os


class TimeSeriesWriter(Protocol):
    def __init__(
        self,
        name: str,
        header: List[str],
        region: str = "",
        dir_name: str = "postProcessing",
    ) -> None:
        pass

    def create_file(self):
        pass

    def append_data(self, data: List[Any]):
        pass


class csvTimeSeries(TimeSeriesWriter):
    def __init__(
        self,
        name: str,
        header: List[str],
        region: str = "",
        dir_name: str = "postProcessing",
    ) -> None:
        self.name = name
        self.header = ["t"] + header
        self.dir_name = dir_name
        self.file_name = Path(self.dir_name, self.name, f"{self.name}.csv")

    def create_file(self):
        with open(self.file_name, "w", encoding="utf8") as f:
            f.write(",".join(self.header))
            f.write(os.linesep)

    def append_data(self, t: float, data: List[Any]):
        with open(self.file_name, "w", encoding="utf8") as f:
            f.write(",".join(data))
            f.write(os.linesep)


def field_aggregation(fields: List[Any], agg_func: List[Callable]) -> List[str]:
    pass


class Force:
    def __init__(self, mesh: fvMesh, bc_names: List[str], p_name: str = "p") -> None:
        self.mesh = mesh
        self.p_name = p_name
        self.bc_names = bc_names

    def calcForces(self) -> Vector:
        p = volScalarField(self.p_name, self.mesh)
        force = Vector(0, 0, 0)
        for bc in self.bc_names:
            force += foam_py.sum(self.mesh.Sf()[bc] * p[bc])

        return force

    def compute(self) -> List[str]:
        force = self.calcForces()
        return [str(force[0]), str(force[1]), str(force[2])]
